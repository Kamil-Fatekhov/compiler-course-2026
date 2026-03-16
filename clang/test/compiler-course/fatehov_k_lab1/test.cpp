// RUN: %clang_cc1 -load %llvmshlibdir/fatehov_k_lab1_ClangAST%pluginext -plugin fatehov_resource_leak -fsyntax-only -I/usr/include/c++/11 -I/usr/include/x86_64-linux-gnu/c++/11 -I/usr/local/include -I/usr/include %s 2>&1 | FileCheck %s

// CHECK: [LEAK DETECTED] Variable 'ptr1' allocated with operator new at [[FILE:.*]]:[[#@LINE+5]]
// CHECK: [LEAK DETECTED] Variable 'ptr3' allocated with operator new at [[FILE]]:[[#@LINE+10]]
// CHECK: [LEAK DETECTED] Variable 'mem1' allocated with malloc/calloc at [[FILE]]:[[#@LINE+15]]
// CHECK: [LEAK DETECTED] Variable 'mem2' allocated with malloc/calloc at [[FILE]]:[[#@LINE+15]]
// CHECK: [LEAK DETECTED] Variable 'mem4' allocated with malloc/calloc at [[FILE]]:[[#@LINE+16]]
// CHECK: [LEAK DETECTED] Variable 'file1' allocated with fopen at [[FILE]]:[[#@LINE+14]]
// CHECK: [LEAK DETECTED] Variable 'file3' allocated with fopen at [[FILE]]:[[#@LINE+14]]
// CHECK: [LEAK DETECTED] Variable 'latePtr' allocated with operator new at [[FILE]]:[[#@LINE+10]]
// CHECK: [LEAK DETECTED] Variable 'lateFile' allocated with fopen at [[FILE]]:[[#@LINE+9]]
// CHECK: [LEAK DETECTED] Variable 'scopePtr' allocated with operator new at [[FILE]]:[[#@LINE+7]]
// CHECK: [LEAK DETECTED] Variable 'loopPtr' allocated with operator new at [[FILE]]:[[#@LINE+7]]
// CHECK: [LEAK DETECTED] Variable 'arr' allocated with operator new at [[FILE]]:[[#@LINE+20]]

#include <cstdlib>
#include <cstdio>
#include <memory>

void testNewLeaks() {
    int* ptr1 = new int(42);           
    int* ptr2 = new int(100);
    delete ptr2;                        
    int* ptr3 = new int[10];            
    // delete[] ptr3;
}

void testMallocLeaks() {
    int* mem1 = (int*)malloc(sizeof(int) * 5);    
    int* mem2 = (int*)calloc(10, sizeof(int));    
    int* mem3 = (int*)malloc(sizeof(int) * 3);
    free(mem3);
    
    char* mem4 = (char*)malloc(100);                
}

void testFileLeaks() {
    FILE* file1 = fopen("data1.txt", "r");        
    FILE* file2 = fopen("data2.txt", "w");
    fclose(file2);
    FILE* file3 = fopen("data3.txt", "a");         
}

void testAssignmentLeaks() {
    int* latePtr;
    latePtr = new int(777);                         
    
    FILE* lateFile;
    lateFile = fopen("late.txt", "r");              
    
    int* goodPtr;
    goodPtr = new int(888);
    delete goodPtr;
}

void testScopeLeaks() {
    if (true) {
        int* scopePtr = new int(555);                
    }
    
    for (int i = 0; i < 3; ++i) {
        int* loopPtr = new int(i);                   
    }
}

void testCleanCode() {
    int* cleanPtr = new int(111);
    delete cleanPtr;
    
    FILE* cleanFile = fopen("clean.txt", "r");
    if (cleanFile) {
        fclose(cleanFile);
    }
    
    int* cleanMem = (int*)calloc(5, sizeof(int));
    free(cleanMem);
    
    std::unique_ptr<int> smartPtr(new int(222));
}

template<typename T>
T* createLeakyArray(int size) {
    T* arr = new T[size];                             
    return arr;
}

void testTemplateLeaks() {
    auto* leakyInts = createLeakyArray<int>(10);
    auto* leakyDoubles = createLeakyArray<double>(5);
}

int main() {
    testNewLeaks();
    testMallocLeaks();
    testFileLeaks();
    testAssignmentLeaks();
    testScopeLeaks();
    testCleanCode();
    testTemplateLeaks();
    return 0;
}