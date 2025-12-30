#include <list>

#include "AppComponent.hpp"
#include "Runner.hpp"

void run() {
  /* Register Components in scope of run() method */
    its::user::AppComponent components(
      {"localhost", 9120},
      {"user.virtualhost", 0});

  /* run */
  std::list<std::thread> acceptingThreads;

  its::user::Runner runner;
  runner.run(&acceptingThreads);

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
