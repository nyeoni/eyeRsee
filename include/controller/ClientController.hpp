#ifndef CLIENTCONTROLLER_HPP
#define CLIENTCONTROLLER_HPP

namespace ft {
class ClientController {
   private:
    /* data */
   public:
    ClientController(/* args*/);
    ClientController(const ClientController &copy);
    ~ClientController();
    ClientController &operator=(const ClientController &ref);
};

}  // namespace ft

#endif
