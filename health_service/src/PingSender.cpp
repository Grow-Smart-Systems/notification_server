#include "PingSender.h"

#include <grpcpp/grpcpp.h>
#include "../build/protoc/health_service.grpc.pb.h"

#include "AsyncLogger.h"

void PingSender::SendPing(ServiceInfo& address) 
{
    if (address.address.empty() || address.port <= 0) 
    {
        std::cerr << "Invalid service address or port: " << address.name << std::endl;
        return;
    }

    std::string fullAddress = address.address + ":" + std::to_string(address.port);
    auto channel = grpc::CreateChannel(fullAddress, grpc::InsecureChannelCredentials());
    std::unique_ptr<HealthService::Stub> stub = HealthService::NewStub(channel);

    PingRequest request;
    request.set_message("ping");

    PingReply reply;
    grpc::ClientContext context;
    
    auto status = stub->Ping(&context, request, &reply);
    std::string log_msg;
    if (status.ok()) 
    {
        std::lock_guard<std::mutex> lock(_serviceInfoMutex);
        log_msg = "[" + getCurrentDateTime() + "] [" + fullAddress + "]\tОтвет: " + reply.message();
        address.status = true;
    } 
    else 
    {
        std::lock_guard<std::mutex> lock(_serviceInfoMutex);
        log_msg = "[" + getCurrentDateTime() + "] [" + fullAddress + "]\tОшибка: " + status.error_message();
        address.status = false;
    }

    if (g_logger) 
        g_logger->log(log_msg);
}

std::string PingSender::getCurrentDateTime()
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    char time_buf[32];
    std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &tm);
    return std::string(time_buf);
}
