#include <stddef.h>
#include <stdlib.h>

#define DEBUG_UNIT_TESTS 0

#if DEBUG_UNIT_TESTS
#include <stdio.h>
#endif

#include <check.h>

START_TEST(check_unit_test_framework_is_operational)
{
    ck_assert_int_eq(0, 0);
}
END_TEST

Suite* check_selftest_suite(void)
{
    Suite* suite = 0;
    TCase* case_all = 0;

    const char* suite_name = "Check selftest";
    const char* case_all_name = "All";

    suite = suite_create(suite_name);
    case_all = tcase_create(case_all_name);

    tcase_add_test(case_all, check_unit_test_framework_is_operational);
    suite_add_tcase(suite, case_all);

    return suite;
}

int main(void)
{
    Suite* suite = 0;
    SRunner* runner = 0;

    int32_t fail_count = 0;

    suite = check_selftest_suite();
    runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);
    fail_count = srunner_ntests_failed(runner);

    srunner_free(runner);

    if (0 != fail_count) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
