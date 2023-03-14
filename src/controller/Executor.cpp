#include "controller/Executor.hpp"

#include "entity/Channel.hpp"

namespace ft {
Executor::Executor() {}

Executor::Executor(const Executor &copy) { *this = copy; }

Executor::~Executor() {}

Executor &Executor::operator=(const Executor &ref) { return (*this); }

/**
 * @brief part channels
 *
 * @param fd
 * @param channels 여러 개의 channel
 */
void Executor::part(int fd, CmdLine channels) {
    Client *target = client_controller.find(fd);
    Channel *channel;
    cmd_iterator iter = channels.begin();

    for (; iter != channels.end(); ++iter) {  // 여러 개의 channel
        // 하나의 channel에서 part
        channel = channel_controller.find(*iter);
        if (channel) {
            client_controller.eraseChannel(target, channel);
            channel_controller.eraseClient(channel, target);
        } else {
            // CHECK there are no channel name -> break?
        }
    }
}

// kenvent.ident -> fd, kevent.udata
// client,channel,
// 새로운 channel 생성
//
void Executor::join(int fd, CmdLine cmd_line) {
    // 0. Client fd로 조회
    // 1. Channel 있는지 확인
    //     1-1.1 Channel 있다면 client 등록 (fd || Client *)
    //     1-1.2 Client에 채널 등록
    //     1-2. 없다면 새로운 channel 생성 및 join
    //     1-2.1 channel 에 client 등록
    //     1-1.2 Client에 채널 등록

    Client *client = client_controller.find(fd);
    cmd_iterator iter = cmd_line.begin();
    for (; iter != cmd_line.end(); iter++) {
        if (iter->front() == '#') {
            Channel *channel = channel_controller.find(*iter);
            if (channel == NULL) {
                channel_controller.create(*iter);
                channel_controller.insertClient(channel, client, true);
            } else {
                channel_controller.insertClient(channel, client, false);
            }
            client_controller.insertChannel(client, channel);
        }
    }
}

void Executor::mode(int fd, std::string channel, e_mode mode) {
    channel_controller.updateMode(mode, channel);
}

void Executor::topic(int fd, std::string channel, std::string topic) {
    Client *client = client_controller.find(fd);
    Channel *target = channel_controller.find(channel);
    if (target && client)
        channel_controller.updateTopic(client, target, topic);
    else {
        // error
    }
}

}  // namespace ft
