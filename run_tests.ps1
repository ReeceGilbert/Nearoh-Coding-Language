$ErrorActionPreference = "Continue"

$exe = ".\cmake-build-debug\nearoh.exe"

$passTests = @(
    "tests/pass/method_return_value.nr",
    "tests/pass/dict_update_lookup.nr",
    "tests/pass/list_index_append.nr",
    "tests/pass/boolean_short_circuit.nr",
    "tests/pass/while_counter.nr",
    "tests/pass/for_loop_function_scope.nr",
    "tests/pass/for_loop_variable_scope.nr",
    "tests/pass/function_return_chain.nr",
    "tests/pass/object_list_mutation.nr",
    "tests/pass/two_instances_separate_state.nr",
    "tests/pass/function_scope.nr",
    "tests/pass/arithmetic_precedence.nr",
    "examples/hello.nr",
    "examples/functions.nr",
    "examples/classes.nr",
    "examples/dictionaries.nr",
    "examples/import_once_main.nr",
    "examples/import_path_normalize_main.nr",
    "examples/modules/main.nr",
    "examples/modules/main_nested.nr",
    "examples/core_stress.nr",
    "examples/arena_showcase.nr",
    "examples/fancy_builtins.nr",
    "examples/string_builtins.nr",
    "examples/string_tools_advanced.nr"
)

$failTests = @(
    @{
        Path = "tests/fail/bad_undefined_member.nr"
        Expected = "Undefined member."
    },
    @{
        Path = "tests/fail/bad_list_index_out_of_bounds.nr"
        Expected = "List index out of bounds."
    },
    @{
        Path = "tests/fail/bad_constructor_arg_count.nr"
        Expected = "Wrong number of arguments for function call."
    },
    @{
        Path = "examples/import_cycle_a.nr"
        Expected = @(
            "Circular import detected."
        )
    },
    @{
        Path = "examples/modules/bad_main.nr"
        Expected = @(
            "examples/modules/bad_utils.nr",
            "Undefined variable."
        )
    },
    @{
        Path = "examples/modules/missing_main.nr"
        Expected = @(
            "Could not read imported file:",
            "examples/modules/missing_utils.nr"
        )
    },
    @{
        Path = "examples/errors/bad_abs.nr"
        Expected = @(
            "abs() expects a number."
        )
    },
    @{
        Path = "examples/errors/bad_floor.nr"
        Expected = @(
            "floor() expects a number."
        )
    },
    @{
        Path = "examples/errors/bad_min_empty.nr"
        Expected = @(
            "min() expects at least 1 argument."
        )
    },
    @{
        Path = "examples/errors/bad_min_type.nr"
        Expected = @(
            "min() expects only numbers."
        )
    },
    @{
        Path = "examples/errors/bad_randint_zero.nr"
        Expected = @(
            "randint() max must be greater than 0."
        )
    },
    @{
        Path = "examples/errors/bad_values.nr"
        Expected = @(
            "values() expects a dictionary."
        )
    },
    @{
        Path = "examples/errors/bad_clock_ms.nr"
        Expected = @(
            "clock_ms() expects exactly 0 arguments but got 1."
        )
    },
    @{
        Path = "examples/errors/bad_lower.nr"
        Expected = @(
            "lower() expects a string."
        )
    },
    @{
        Path = "examples/errors/bad_contains.nr"
        Expected = @(
            "contains() expects the second argument to be a string."
        )
    },
    @{
        Path = "examples/errors/bad_substr_start.nr"
        Expected = @(
            "substr() start index out of bounds."
        )
    },
    @{
        Path = "examples/errors/bad_substr_length.nr"
        Expected = @(
            "substr() length cannot be negative."
        )
    },
    @{
        Path = "examples/errors/bad_split_empty_separator.nr"
        Expected = @(
            "split() separator cannot be empty."
        )
    },
    @{
        Path = "examples/errors/bad_join_non_list.nr"
        Expected = @(
            "join() expects the first argument to be a list."
        )
    },
    @{
        Path = "examples/errors/bad_join_non_string_item.nr"
        Expected = @(
            "join() expects a list containing only strings."
        )
    },
    @{
        Path = "examples/errors/bad_replace_empty_old.nr"
        Expected = @(
            "replace() old text cannot be empty."
        )
    }
)

foreach ($test in $passTests) {
    Write-Host ""
    Write-Host "=== PASS TEST $test ==="

    $output = & $exe $test
    $exitCode = $LASTEXITCODE

    $output | ForEach-Object { Write-Host $_ }

    if ($exitCode -ne 0) {
        Write-Host "FAIL: $test should have passed"
        exit 1
    }

    $testName = [System.IO.Path]::GetFileNameWithoutExtension($test)
    $expectedPath = "tests/expected/$testName.txt"

    if (Test-Path $expectedPath) {
        $expected = Get-Content $expectedPath

        if (($output -join "`n") -ne ($expected -join "`n")) {
            Write-Host "FAIL: output mismatch for $test"
            Write-Host ""
            Write-Host "Expected:"
            $expected | ForEach-Object { Write-Host $_ }
            Write-Host ""
            Write-Host "Actual:"
            $output | ForEach-Object { Write-Host $_ }
            exit 1
        }
    }
}

foreach ($test in $failTests) {
    Write-Host ""
    Write-Host "=== EXPECTED FAILURE TEST $($test.Path) ==="

    $output = & $exe $test.Path 2>&1
    $exitCode = $LASTEXITCODE
    $text = $output -join "`n"

    Write-Host $text

    if ($exitCode -eq 0) {
        Write-Host ""
        Write-Host "FAILED: expected failure but got success"
        Write-Host "TEST: $($test.Path)"
        exit 1
    }

    foreach ($expectedText in $test.Expected) {
        if ($text -notlike "*$expectedText*") {
            Write-Host ""
            Write-Host "FAILED: expected error output to contain:"
            Write-Host $expectedText
            Write-Host ""
            Write-Host "Actual output:"
            Write-Host $text
            exit 1
        }
    }
}

Write-Host ""
Write-Host "ALL TESTS PASSED"