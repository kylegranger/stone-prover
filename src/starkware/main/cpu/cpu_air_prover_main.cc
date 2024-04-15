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
#include <omp.h>

extern "C" {
  #include "third_party/gevulot/shim.h"
}


void show_dir_content(char * path)
{
  printf("contents for %s\n", path);

    DIR * d = opendir(path); // open the path
  if(d==NULL) return; // if was not able, return
  struct dirent * dir; // for the directory entries
  while ((dir = readdir(d)) != NULL) // if we were able to read somehting from the directory
    {
      if(dir-> d_type != DT_DIR) // if the type is not directory just print it with blue color
        printf("%s\n", dir->d_name);
      else
      if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 ) // if it is a directory
      {
        printf("%s\n", dir->d_name); // print its name in green
        char d_path[255]; // here I am using sprintf which is safer than strcat
        sprintf(d_path, "%s/%s", path, dir->d_name);
        show_dir_content(d_path); // recall with the new path
      }
    }
    closedir(d); // finally close the directory
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

void* gevulot_stone_prover(const struct Task* task) {
  printf("gevulot_stone_prover: task id: %s\n", task->id);
  printf("gevulot_stone_prover: Args: \n");
  char ** args = (char**)task->args;
  int nargs = 0;
  std::vector<std::string> vargs;
  std::string s;
  vargs.push_back("cpu_air_prover");
  vargs.push_back("--logtostderr");
  char *out_file = (char *)"/workspace/proof.json";
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

  auto res = chdir("/workspace");
  printf("chdir: %d", res);
  run_main(argc, argv);

  printf("gevulot_stone_prover: Done with task.\n");

  void *result =  new_task_result(NULL, 0);
  add_file_to_result(result, out_file);
  return result;
}

#if 1
int main(int argc, char **argv)
{
  printf("cpu_air_prover_main::main(): argc %d\n", argc);
  printf("  Args: \n");
  for (int i = 0; i < argc; i++) {
    printf("    %d, %s\n", i, argv[i]);
  }

    show_dir_content((char *)".");
    show_dir_content((char *)"/workspace");


    auto ncpus = std::thread::hardware_concurrency();
    printf("std::thread::hardware_concurrency %d\n", ncpus);

    int nThreads = omp_get_num_threads();
    printf("omp_get_num_threads A %d\n", nThreads);

    // omp_set_num_threads(ncpus);
    // nThreads = omp_get_num_threads();
    // printf("omp_get_num_threads B %d\n", nThreads);

    printf("do parallel omg run\n");
    #pragma omp parallel
    {
        int idThread = omp_get_thread_num();
        int nThreads = omp_get_num_threads();
        printf("idThread %d\n", idThread);
        printf("nThreads %d\n", nThreads);
    }
    printf("done parallel omg run\n");

    // printf("get omp_get_max_threads\n");
    // auto nthreads = omp_get_max_threads();
    // printf("omp_get_max_threads = %d\n", nthreads);
    // printf("get omp_get_max_threads");
    // // auto nprocs = omp_get_num_procs();
    // printf("omp_get_num_procs = %d", nprocs);



  if (argc == 1) {
    run(gevulot_stone_prover);
  } else {
    run_main(argc, argv);
  }

  printf("cpu_air_prover_main::main(): Terminating...\n");
  return 0;
}
#else

int main(int argc, char **argv)
{
  printf("openmp_test::main(): argc %d\n", argc);
  printf("  Args: \n");
  for (int i = 0; i < argc; i++) {
    printf("    %d, %s\n", i, argv[i]);
  }

    // show_dir_content((char *)".");
    // show_dir_content((char *)"/workspace");
    show_dir_content((char *)"./gevulot");


    auto ncpus = std::thread::hardware_concurrency();
    printf("std::thread::hardware_concurrency %d\n", ncpus);

    int nThreads = omp_get_num_threads();
    printf("omp_get_num_threads A %d\n", nThreads);

    // omp_set_num_threads(ncpus);
    // nThreads = omp_get_num_threads();
    // printf("omp_get_num_threads B %d\n", nThreads);

    printf("do parallel omg run\n");
    #pragma omp parallel
    {
        int idThread = omp_get_thread_num();
        // int nThreads = omp_get_num_threads();
        printf("idThread %d ", idThread);
        // printf("nThreads %d\n", nThreads);
    }
    printf("\ndone parallel omg run\n");

    printf("get omp_get_max_threads\n");
    auto nthreads = omp_get_max_threads();
    printf("omp_get_max_threads = %d\n", nthreads);
    printf("get omp_get_num_procs\n");
    auto nprocs = omp_get_num_procs();
    printf("omp_get_num_procs = %d\n", nprocs);



//   if (argc == 1) {
//     run(gevulot_stone_prover);
//   } else {
//     run_main(argc, argv);
//   }

  printf("openmp_test::main(): Terminating...\n");
  return 0;
}

#endif