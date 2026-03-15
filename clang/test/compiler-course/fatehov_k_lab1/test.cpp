// RUN: %clang_cc1 -load %llvmshlibdir/fatehov_k_lab1_ClangAST%pluginext -plugin fatehov_resource_leak -fsyntax-only %s 2>&1 | FileCheck %s

#include <cstdlib>
#include <cstdio>

// CHECK: [LEAK DETECTED] Variable 'ptr1' allocated with operator new at {{.*}}:[[#__LINE__+3]]
void testNewLeaks() {
    int* ptr1 = new int(42);           
    int* ptr2 = new int(100);
    delete ptr2;                        
    int* ptr3 = new int[10];            
    // delete[] ptr3;                   
}

// CHECK: [LEAK DETECTED] Variable 'mem1' allocated with malloc/calloc at {{.*}}:[[#__LINE__+4]]
void testMallocLeaks() {
    int* mem1 = (int*)malloc(sizeof(int) * 5);    
    int* mem2 = (int*)calloc(10, sizeof(int));    
    int* mem3 = (int*)malloc(sizeof(int) * 3);
    free(mem3);                                     
    
    char* mem4 = (char*)malloc(100);                
}

// CHECK: [LEAK DETECTED] Variable 'file1' allocated with fopen at {{.*}}:[[#__LINE__+3]]
void testFileLeaks() {
    FILE* file1 = fopen("data1.txt", "r");        
    FILE* file2 = fopen("data2.txt", "w");
    fclose(file2);                                  
    FILE* file3 = fopen("data3.txt", "a");         
}

// CHECK: [LEAK DETECTED] Variable 'latePtr' allocated with operator new at {{.*}}:[[#__LINE__+4]]
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
        // CHECK: [LEAK DETECTED] Variable 'scopePtr' allocated with operator new at {{.*}}:[[#__LINE__-2]]
    }
    
    for (int i = 0; i < 3; ++i) {
        int* loopPtr = new int(i);                   
        // CHECK: [LEAK DETECTED] Variable 'loopPtr' allocated with operator new at {{.*}}:[[#__LINE__-1]]
    }
}

// CHECK-NOT: [LEAK DETECTED] Variable 'cleanPtr' 
void testCleanCode() {
    int* cleanPtr = new int(111);
    delete cleanPtr;
    
    FILE* cleanFile = fopen("clean.txt", "r");
    if (cleanFile) {
        // ... операции
        fclose(cleanFile);
    }
    
    int* cleanMem = (int*)calloc(5, sizeof(int));
    free(cleanMem);
    
    std::unique_ptr<int> smartPtr(new int(222));
}

template<typename T>
T* createLeakyArray(int size) {
    T* arr = new T[size];                            
    // CHECK: [LEAK DETECTED] Variable 'arr' allocated with operator new at {{.*}}:[[#__LINE__-1]]
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