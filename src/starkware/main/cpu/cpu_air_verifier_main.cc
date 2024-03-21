// Copyright 2023 StarkWare Industries Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License").
// You may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// https://www.starkware.co/open-source-license/
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions
// and limitations under the License.

/*
  Verifies a CPU AIR proof. See cpu_air_prover_main.cc for details.
*/

#include <optional>

#include "gflags/gflags.h"

#include "starkware/main/verifier_main_helper.h"
#include "starkware/statement/cpu/cpu_air_statement.h"
#include "starkware/utils/json.h"

extern "C" {
  #include "third_party/gevulot/shim.h"
}

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int run_main(int argc, char** argv) {
  using namespace starkware;       // NOLINT
  using namespace starkware::cpu;  // NOLINT
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);  // NOLINT

  auto factory = [](const JsonValue& public_input, const JsonValue& parameters) {
    return std::unique_ptr<Statement>(
        new CpuAirStatement(parameters["statement"], public_input, std::nullopt));
  };
  bool result = starkware::VerifierMainHelper(factory);

  if (result) {
    LOG(INFO) << "Proof verified successfully.";
  } else {
    LOG(ERROR) << "Invalid proof.";
  }

  return result ? 0 : 1;
}


void* gevulot_stone_verifier(const struct Task* task) {
  printf("gevulot_stone_verifier: task id: %s\n", task->id);
  printf("gevulot_stone_verifier: Args: \n");
  char ** args = (char**)task->args;
  int nargs = 0;
  std::vector<std::string> vargs;
  std::string s;
  vargs.push_back("cpu_air_verifier");
  vargs.push_back("--logtostderr");
  char *in_file = nullptr;
  while ((args != nullptr) && (*args != nullptr)) {
    if (s.compare("--in_file=") == 0) {
        in_file = *args;
    }
    printf("  %s\n", *args);
    s += *args;
    if (nargs & 1) {
        vargs.push_back(s);
        s = "";
    } else {
        s += "=";
    }
    args++;
    nargs++;
  }

  printf("gevulot_stone_verifier: vargs size = %zu\n", vargs.size());
  auto argv = new char *[vargs.size()];
  int argc = 0;
  for (size_t i = 0; i < vargs.size(); i++) {
    argv[i] = (char *)vargs[i].c_str();
    printf("  varg %zu is %s\n", i, argv[i]);
    argc++;
  }

  auto res  = run_main(argc, argv);

  bool is_success = res == 0;
  std::string message = is_success ? "Starkware verifier result: success" : "Starkware verifier result: fail";
  uint64_t timestamp = timeSinceEpochMillisec();
  std::string proof_file = in_file;

  Json::Value verifier_result;
  verifier_result["is_success"] = is_success;
  verifier_result["message"] = message;
  verifier_result["timestamp"] = timestamp;
  verifier_result["proof_file"] = proof_file;
  auto jresult = verifier_result.toStyledString();
  auto jlength = strlen(jresult.c_str());
  printf("  jresult %s has length %zu\n", jresult.c_str(), jlength);

  printf("gevulot_stone_verifier: Done with the task.\n");

  return new_task_result((uint8_t *)jresult.c_str(), jlength);
}

int main(int argc, char **argv)
{
  printf("cpu_air_verifier_main::main(): argc %d\n", argc);
  printf("  Args: \n");
  for (int i = 0; i < argc; i++) {
    printf("    %d, %s\n", i, argv[i]);
  }
  if (argc == 1) {
    run(gevulot_stone_verifier);
  } else {
    run_main(argc, argv);
  }

  printf("cpu_air_verifier_main::main(): Terminating...\n");
  return 0;
}

