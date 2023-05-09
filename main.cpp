#include "tests.h"
#include "simple_vector.h"
int main() {
    std::cout << "Start: Testing..." << std::endl;
    TestInitialization();
    TestMethods();
    TestTemporaryObjConstructor();
    TestTemporaryObjOperator();
    TestNamedMoveConstructor();
    TestNamedMoveOperator();
    TestNoncopiableMoveConstructor();
    TestNoncopiablePushBack();
    TestNoncopiableInsert();
    TestNoncopiableErase();
    std::cout << "End: All tests passed successfully.";
    return 0;
}