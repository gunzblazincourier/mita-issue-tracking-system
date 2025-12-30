/**
 * @file Runner.hpp
 * @brief Declares the server runner for starting acceptors.
 */
#ifndef its_user_Runner_hpp
#define its_user_Runner_hpp

#include <list>
#include <thread>

#include "oatpp/web/server/api/ApiController.hpp"

namespace its { namespace user {

/**
 * @brief Starts and manages server accepting threads.
 */
class Runner {
 public:
  Runner();
  void run(std::list<std::thread>* acceptingThreads);
};

}} // namespace user


#endif // its_user_Runner_hpp
