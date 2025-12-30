/**
 * @file Runner.hpp
 * @brief Declares the issue server runner for acceptor threads.
 */
#ifndef its_issue_Runner_hpp
#define its_issue_Runner_hpp

#include <list>
#include <thread>

#include "oatpp/web/server/api/ApiController.hpp"

namespace its { namespace issue {

/**
 * @brief Starts and manages server accepting threads.
 */
class Runner {
 public:
  Runner();

  void run(std::list<std::thread>& acceptingThreads);
};
}}
#endif // its_user_Runner_hpp
