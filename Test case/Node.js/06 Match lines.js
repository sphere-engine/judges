/**
 * Test case judge "Match lines" for Sphere Engine Problems
 *
 *
 * The judge iterates over lines of the test case output file and compares them
 * with corresponding lines from the program's output file.
 *
 * A submission is considered as correct (i.e. "accepted") if there is at least
 * a single pair of identical corresponding lines.
 *
 * The score is calculated as a rounded percentage value of matched lines.
 *
 * EXAMPLE
 * Test case output file:      Program's output
 * 1                           1
 * 2                           4
 * 3                           3
 * 4                           2
 * 5                           5
 *
 * Status: accepted
 * Score: 60 (out of 100)
 *
 * @copyright  Copyright (c) 2018 Sphere Research Labs (http://sphere-engine.com)
 */

const fs = require('fs');

// file descriptors
const TEST_CASE_INPUT_FD = 0;
const TEST_CASE_OUTPUT_FD = 4;
const PROGRAM_SOURCE_FD = 5;
const PROGRAM_OUTPUT_FD = 3;
const PROGRAM_INPUT_FD = 8; // interactive only
const SCORE_FD = 1;
const DEBUG_FD = 6;

// statuses
const ACCEPTED = 0;
const WRONG_ANSWER = 1;

// read data from streams
var test_case_input = fs.readFileSync(TEST_CASE_INPUT_FD).toString().split('\n');
var test_case_output = fs.readFileSync(TEST_CASE_OUTPUT_FD).toString().split('\n');
var program_source = fs.readFileSync(PROGRAM_SOURCE_FD).toString().split('\n');
var program_output = fs.readFileSync(PROGRAM_OUTPUT_FD).toString().split('\n');

////////////////////////////////////////////////////////////////////////////////

var number_of_lines = test_case_output.length;
var number_of_correct_lines = 0

for (var i = 0; i < number_of_lines; i++) {
    test_case_output_line = test_case_output[i]

    if (i >= program_output.length) {
        fs.writeSync(DEBUG_FD,
            'Outputs differ on line ' + i + '. Expected "' + test_case_output_line + '" but program\'s output has ended before.\n');
        continue;
    }

    program_output_line = program_output[i]

    if (test_case_output_line == program_output_line) {
        number_of_correct_lines += 1
    } else {
        fs.writeSync(DEBUG_FD,
            'Outputs differ on line ' + i + '. Expected "' + test_case_output_line +
            '" but was "' + program_output_line + '".\n');
    }
}

// determine result (i.e. score and status)
var score = (number_of_lines == 0) ? 100 : 100 * number_of_correct_lines / number_of_lines;
var status = (score > 0) ? ACCEPTED : WRONG_ANSWER;

// set score
fs.writeSync(SCORE_FD, score.toString());

// exit with suitable status
process.exit(status);
