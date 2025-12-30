#include <list>

#include "AppComponent.hpp"
#include "Runner.hpp"

void run() {
  /* Register Components in scope of run() method */
  its::issue::AppComponent components(
      {"localhost", 9100},  // Issue service (this)
      {"localhost", 9120}); // User service

  /* run */
  std::list<std::thread> acceptingThreads;
  its::issue::Runner runner;
  runner.run(acceptingThreads);

  for (auto& thread : acceptingThreads) {
    thread.join();
  }
}

int main(int argc, const char* argv[]) {
  oatpp::base::Environment::init();
  run();
  oatpp::base::Environment::destroy();

  return 0;
}
