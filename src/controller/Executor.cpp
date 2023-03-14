#include "controller/Executor.hpp"

namespace ft {
Executor::Executor() {}

Executor::Executor(const Executor &copy) { *this = copy; }

Executor::~Executor() {}

Executor &Executor::operator=(const Executor &ref) { return (*this); }

//// /quit -> 모든 채널에서 해당 client 삭제
// void Executor::deleteClient(std::string nickname) {
//     // find -> Client -> channels

//    Client *client = client_controller.find(nickname);
//    ChannelList channels;

//    if (client != NULL) {
//        client_controller.del(nickname);
//        channels = client.getChannels();  // channel list
//        // int channel_cnt = client.getChannels().size();

//        for (channel_iterator i = channels.begin(); i != channels.end(); ++i)
//        {
//            // 해당 client가 가입된 channel에서 client 삭제
//            channel_controller.delete();
//        }
//    }
//    // client_controller channel list -> delete
//    // channel_controller.delete();
//}

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
                channel_controller.updateChannel(channel, client, true,
                                                 true);  // oeprator
            } else {
                channel_controller.updateChannel(channel, client, false,
                                                 true);  // regular
            }
            client_controller.updateClient(client, channel, true);
        }
    }
}

void Executor::mode(int fd, std::string channel, e_mode mode) {
    channel_controller.update(mode, channel);
}

}  // namespace ft
