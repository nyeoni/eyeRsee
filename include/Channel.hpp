#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Channel {
   private:
    /* data */
   public:
    Channel(/* args*/);
    Channel(const Channel &copy);
    ~Channel();
    Channel &operator=(const Channel &ref);
};

#endif