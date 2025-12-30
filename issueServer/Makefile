CXX=g++
CXXFLAGS=

OATPP_SWAGGER_INCLUDE = /usr/local/include/oatpp-1.3.0/oatpp-swagger
OATPP_INCLUDE_LIB = /usr/local/include/oatpp-1.3.0/oatpp
OATPP_INCLUDE = -I src -I $(OATPP_INCLUDE_LIB) -I $(OATPP_SWAGGER_INCLUDE)

LINKFLAGS_LOCAL = -L /usr/local/lib/oatpp-1.3.0
LINKFLAGS_APP =  $(LINKFLAGS_LOCAL) -loatpp-swagger -loatpp

SRC_DIR = src
SRC_DIR_SERVER = src/server
SRC_DIR_DTO = src/dto
SRC_DIR_CONTROLLER = src/controller
SRC_DIR_SERVICE_ISSUE = src/service/issue
SRC_DIR_SERVICE_USER    = src/service/user
SRC_DIR_SERVICE_COMMENT = src/service/comment

GCOV = gcov
LCOV = lcov
COVERAGE_RESULTS = coverage/coverage.info
COVERAGE_DIR = coverage

STATIC_ANALYSIS = cppcheck

STYLE_CHECK = cpplint

PROGRAM_SERVER = issueServer
DOXY_DIR = docs/code
DOXYFILE = $(DOXY_DIR)/doxyfile

.PHONY: all
all: $(PROGRAM_SERVER) coverage docs static style

# default rule for compiling .cc to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean-cov
clean-cov:
	rm -rf *.gcov *.gcda *.gcno

.PHONY: clean-temp
clean-temp:
	rm -rf *~ \#* .\#* \
		$(SRC_DIR)/*~ $(SRC_DIR)/\#* $(SRC_DIR)/.\#* \
		*.gcov *.gcda *.gcno

.PHONY: clean
clean:
	rm -rf *~ $(SRC)/*.o *.gcov *.gcda *.gcno \
	$(COVERAGE_RESULTS) \
	$(PROGRAM_SERVER) \
	$(COVERAGE_DIR) \
	doxygen/html \
	obj bin \

server: $(PROGRAM_SERVER)

$(PROGRAM_SERVER): $(SRC_DIR_SERVER) $(SRC_DIR_SERVICE)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM_SERVER) $(OATPP_INCLUDE) \
	$(SRC_DIR_SERVER)/*.cpp \
	$(SRC_DIR_DTO)/*.hpp \
	$(SRC_DIR_CONTROLLER)/*.hpp \
  $(SRC_DIR_SERVICE_ISSUE)/*.hpp \
  $(SRC_DIR_SERVICE_USER)/*.hpp \
  $(SRC_DIR_SERVICE_COMMENT)/*.hpp \
	$(LINKFLAGS_APP)

.PHONY: coverage
coverage: clean-cov
	@rm -f $(PROGRAM_SERVER)
	@if [ -f "$(OATPP_INCLUDE_LIB)/oatpp/core/macro/codegen.hpp" ]; then \
		$(MAKE) -B CXXFLAGS="$(CXXFLAGS) -fprofile-arcs -ftest-coverage -O0 -g" $(PROGRAM_SERVER); \
	else \
		echo "Skipping build: oatpp headers missing on runner"; \
	fi
	@test -d $(COVERAGE_DIR) || mkdir -p $(COVERAGE_DIR)
	${LCOV} --capture --initial --gcov-tool ${GCOV} --directory . --output-file ${COVERAGE_RESULTS} --rc lcov_branch_coverage=1
	${LCOV} --extract ${COVERAGE_RESULTS} */*/*/${SRC_DIR}/* -o ${COVERAGE_RESULTS} || true
	genhtml ${COVERAGE_RESULTS} --output-directory ${COVERAGE_DIR} || true
	@test -f $(COVERAGE_DIR)/index.html || echo "No coverage data (headers missing or no .gcno)" > $(COVERAGE_DIR)/index.html
	$(MAKE) clean-temp

docker:
	docker build --pull --rm -f "Dockerfile" -t issue-tracking-system:latest "."

start:
	docker compose -f "compose.yml" up -d --build

stop:
	docker compose -f "compose.yml" down

style: ${SRC_DIR_SERVICE} ${SRC_INCLUDE}
	${STYLE_CHECK} src/controller/* src/dto/* src/server/* src/service/*/*

.PHONY: docs
docs:
	@if [ -f "$(DOXYFILE)" ]; then \
		doxygen $(DOXYFILE); \
	else \
		mkdir -p docs/code/html; \
		echo "<html><body><h1>API Docs</h1><p>No doxygen config found.</p></body></html>" > docs/code/html/index.html; \
	fi