#include "controller/Executer.hpp"

namespace ft {
Executer::Executer() {}

Executer::Executer(const Executer &copy) { *this = copy; }

Executer::~Executer() {}

Executer &Executer::operator=(const Executer &ref) { return (*this); }

// /quit
void Executer::deleteClient(std::string nickname) {
    // find -> Client -> channels

    Client *client = client_controller.find(nickname);
    std::set<Channel *> channels;

    if (client != NULL) {
        client_controller.delete(nickname);
        channels = client.getChannels();
        // int channel_cnt = client.getChannels().size();

        for (channel_iterator i = channels.begin(); i != channels.end(); ++i) {
            // 해당 client가 가입된 channel에서 client 삭제
            channel_controller.delete();
        }
    }
    // client_controller channel list -> delete
    // channel_controller.delete();
}

}  // namespace ft
