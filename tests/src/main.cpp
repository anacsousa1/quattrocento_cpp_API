#include <future>
#include <inttypes.h>
#include <memory>
#include <Sockets/ClientSocket.hpp>
#include <stdio.h>
#include <stdlib.h>

namespace {
    constexpr uint32_t address = 0xa9fe010a;
    constexpr uint16_t port = 23456;

    void OnReceiveMessage(const std::string& message) {
        printf("Received message: %s\n", message.c_str());
    }

}

int main(int argc, char* argv[]) {
    Sockets::ClientSocket client;
    if (!client.Bind()) {
        return EXIT_FAILURE;
    }

    auto closed = std::make_shared< std::promise< void > >();
    if (
        !client.Connect(
            address,
            port,
            OnReceiveMessage,
            [closed]{ closed->set_value(); }
        )
    ) {
        return EXIT_FAILURE;
    }

    printf("Connected and sending message to port %" PRIu16 "...\n", port);
    client.SendMessage("Hello, World!");
    client.Close();

    closed->get_future().wait();
    printf("Program exiting.\n");
    return EXIT_SUCCESS;
}