//
//  Wasm3 - high performance WebAssembly interpreter written in C.
//
//  Copyright © 2019 Steven Massey, Volodymyr Shymanskyy.
//  All rights reserved.
//

#include "source/wasm3.h"
#include "m3_core.h"
#include "source/extra/wasm_from_factory.h"

#include <stdio.h>

#define FATAL(func, msg) {           \
  puts("Fatal: " func ": ");         \
  puts(msg); return 1; }

static IM3Environment env;
static IM3Runtime runtime;
static IM3Module module;

int init_wasm()
{
    M3Result result = m3Err_none;
    unsigned char* bin_array = wasm_binary_array;
    unsigned int sizeptr = wasm_binary_len;
    size_t fsize = (size_t)sizeptr;
    
    uint8_t* wasm = (uint8_t*)bin_array;

    puts("Loading WebAssembly...");

    env = m3_NewEnvironment ();
    if (!env) FATAL("m3_NewEnvironment", "failed");

    puts("Creating Runtime");
    runtime = m3_NewRuntime (env, 1000000, NULL);
    if (!runtime) FATAL("m3_NewRuntime", "failed");

    puts("Parsing Module");
    result = m3_ParseModule (env, &module, wasm, fsize);
    if (result) FATAL("m3_ParseModule", result);

    puts("Loading Module");
    result = m3_LoadModule (runtime, module);
    if (result) FATAL("m3_LoadModule", result);
    return 0;
}

int run_wasm(const char func_name[], int arr[], int l, int r)
{
    M3Result result = m3Err_none;
    unsigned char* bin_array = wasm_binary_array;
    unsigned int sizeptr = wasm_binary_len;
    size_t fsize = (size_t)sizeptr;
    
    uint8_t* wasm = (uint8_t*)bin_array;

    puts("Loading WebAssembly...");

    IM3Environment env = m3_NewEnvironment ();
    if (!env) FATAL("m3_NewEnvironment", "failed");

    puts("Creating Runtime");
    IM3Runtime runtime = m3_NewRuntime (env, 1000000, NULL);
    if (!runtime) FATAL("m3_NewRuntime", "failed");

    puts("Parsing Module");
    IM3Module module;
    result = m3_ParseModule (env, &module, wasm, fsize);
    if (result) FATAL("m3_ParseModule", result);

    puts("Loading Module");
    result = m3_LoadModule (runtime, module);
    if (result) FATAL("m3_LoadModule", result);

    puts("Finding function");
    IM3Function merge_sort;
    IM3Function walloc;
    result = m3_FindFunction (&merge_sort, runtime, func_name);
    if (result) FATAL("m3_FindFunction", result);

    puts("Running...");

    result = m3_CallV (merge_sort, arr, l, r);
    if (result) FATAL("m3_Call", result);

    uint32_t value = 0;
    result = m3_GetResultsV (merge_sort, &value);
    if (result) FATAL("m3_GetResults", result);

    puts("Result: ");
    printf("%d\n", value);
    return 0;
}

void printArray(int A[], int size) 
{ 
    int i; 
    for (i = 0; i < size; i++)
    {
      printf("%d ", A[i]);
    }
    puts("\n");
} 

int init_func(IM3Function *func ,const char name[])
{
  M3Result result = m3Err_none;
  result = m3_FindFunction (func, runtime, name);
  if (result) FATAL("m3_FindFunction", result);
  return 0;
}

int * walloc(IM3Function *func, size_t size)
{
  M3Result result = m3Err_none;
  result = m3_CallV (*func, size);
  if (result) FATAL("m3_Call", result);
  int *ptr;
  result = m3_GetResultsV (*func, &ptr);
  if (result) FATAL("m3_GetResults", result);
  return ptr;
}

int merge_sort(IM3Function *func, int *warray, int l, int r)
{
  M3Result result = m3Err_none;
  result = m3_CallV (*func, warray, l, r);
  if (result) FATAL("m3_Call", result);

  result = m3_GetResultsV (*func);
  if (result) FATAL("m3_GetResults", result);
  return 0;
}

int main()
{
  M3Result result = m3Err_none;
  int arr[] = { 12, 11, 13, 5, 6, 7 };
  int arr_size = sizeof(arr) * sizeof(arr[0]); 
  init_wasm();

  puts("init walloc");
  IM3Function _walloc;
  result = m3_FindFunction (&_walloc, runtime, "walloc");
  if (result) FATAL("m3_FindFunction", result);
  puts("init mergeSort");
  IM3Function _merge_sort;
  result = m3_FindFunction (&_merge_sort, runtime, "mergeSort");
  if (result) FATAL("m3_FindFunction", result);

  puts("Alloc on wasm stack");
  int *warray;
  warray = walloc(&_walloc, arr_size + 20);
  if (warray == NULL){
    printf("Memoy allocation failed");
    return 1;
  }

  uint32_t memSize = m3_GetMemorySize(runtime); 
  uint32_t *ptrMem = m3_GetMemory(runtime, &memSize, 0);

  printf("pointeur stack : %p\n", ptrMem); //  140315139727384
  printf("pointeur warray : %p\n", warray); // 70912
  puts("Copy on wasm stack");
  for (int i=0; i<arr_size; i++){
    warray[i] = arr[i];
  }

  int *array;
  array = (int *)malloc(arr_size);

  puts("sort on wasm stack");
  merge_sort(&_merge_sort, warray, 0, arr_size - 1);
  printArray(warray, arr_size);
  return 0;
}

