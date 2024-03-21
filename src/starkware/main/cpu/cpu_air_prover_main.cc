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
  Creates a proof for the claim:
    I executed a program with a given initial memory (which includes the program and its public
    input). It ran successfully and ended at the given pc.
*/

#include "gflags/gflags.h"

#include "starkware/main/prover_main_helper.h"
#include "starkware/main/prover_version.h"
#include "starkware/statement/cpu/cpu_air_statement.h"
#include "starkware/utils/profiling.h"
#include "starkware/utils/stats.h"
#include <dirent.h>

extern "C" {
  #include "third_party/gevulot/shim.h"
}


int run_main(int argc, char** argv) {
  using namespace starkware;       // NOLINT
  using namespace starkware::cpu;  // NOLINT
  gflags::SetVersionString(GetProverVersionString());
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);  // NOLINT

  CpuAirStatement statement(GetParametersInput()["statement"], GetPublicInput(), GetPrivateInput());
  ProfilingBlock profiling_block("Prover", 0);
  ProverMainHelper(&statement, GetProverVersion());
  WriteStats();

  return 0;
}

// void print_dirs() {

//   DIR *d;
//   struct dirent *dir;
//   d = opendir(".");
//   if (d) {
//     while ((dir = readdir(d)) != NULL) {
//       printf(" in . : %s\n", dir->d_name);
//     }
//     closedir(d);
//   }
//   d = opendir("/workspace");
//   if (d) {
//     while ((dir = readdir(d)) != NULL) {
//       printf(" in /workspace : %s\n", dir->d_name);
//     }
//     closedir(d);
//   }

//   printf("now change directory into /workspace");
//   auto res = chdir("/workspace");
//   printf("  res %d", res);

//   d = opendir(".");
//   if (d) {
//     while ((dir = readdir(d)) != NULL) {
//       printf(" in . : %s\n", dir->d_name);
//     }
//     closedir(d);
//   }

// }

void* gevulot_stone_prover(const struct Task* task) {
  printf("gevulot_stone_prover: task id: %s\n", task->id);
  printf("gevulot_stone_prover: Args: \n");
  char ** args = (char**)task->args;
  int nargs = 0;
  std::vector<std::string> vargs;
  std::string s;
  vargs.push_back("cpu_air_prover");
  vargs.push_back("--logtostderr");
  char *out_file = NULL;
  while ((args != NULL) && (*args != NULL)) {
    printf("  %s\n", *args);
    if (s.compare("--out_file=") == 0) {
        out_file = *args;
    }
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

  printf("gevulot_stone_prover: num arguments = %d\n", nargs);
  printf("gevulot_stone_prover: vargs size = %zu\n", vargs.size());
  auto argv = new char *[vargs.size()];
  int argc = 0;
  for (size_t i = 0; i < vargs.size(); i++) {
    argv[i] = (char *)vargs[i].c_str();
    printf("  varg %zu is %s\n", i, argv[i]);
    argc++;
  }

  // print_dirs();
  auto res = chdir("/workspace");
  printf("chdir: %d", res);
  run_main(argc, argv);

  printf("gevulot_stone_prover: Done with the task.\n");

  void *result =  new_task_result(NULL, 0);
  add_file_to_result(result, out_file);
  return result;
}

int main(int argc, char **argv)
{
  printf("cpu_air_prover_main::main(): argc %d\n", argc);
  printf("  Args: \n");
  for (int i = 0; i < argc; i++) {
    printf("    %d, %s\n", i, argv[i]);
  }

  if (argc == 1) {
    run(gevulot_stone_prover);
  } else {
    run_main(argc, argv);
  }

  printf("cpu_air_prover_main::main(): Terminating...\n");
  return 0;
}

