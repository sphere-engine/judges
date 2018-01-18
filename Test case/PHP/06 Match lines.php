<?php
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

// streams
$streams = [
    // reading
    'test_case_input' => fopen('php://fd/' . TEST_CASE_INPUT_FD , "r"),
    'test_case_output' => fopen('php://fd/' . TEST_CASE_OUTPUT_FD , "r"),
    'program_source' => fopen('php://fd/' . PROGRAM_SOURCE_FD , "r"),
    'program_output' => fopen('php://fd/' . PROGRAM_OUTPUT_FD , "r"),
    // writing
    'program_input' => fopen('php://fd/' . PROGRAM_INPUT_FD , "w"), // interactive only
    'score' => fopen('php://fd/' . SCORE_FD , "w"),
    'debug' => fopen('php://fd/' . DEBUG_FD , "w"),
];

function close_streams() {
    global $streams;
    foreach ($streams as $s) {
        fclose($s);
    }
}

////////////////////////////////////////////////////////////////////////////////

$number_of_lines = 0;
$number_of_correct_lines = 0;

while (($test_case_output_line = fgets($streams['test_case_output'])) !== false) {
    $test_case_output_line = rtrim($test_case_output_line, "\n");
    $program_output_line = fgets($streams['program_output']);
    $number_of_lines++;

    if ($program_output_line === false) {
        fprintf($streams['debug'],
            "Outputs differ on line %d. Expected \"%s\" but program's output has ended before.\n",
            $number_of_lines, $test_case_output_line);
        continue;
    }

    $program_output_line = rtrim($program_output_line, "\n");

    if ($test_case_output_line == $program_output_line) {
        $number_of_correct_lines++;
    } else {
        fprintf($streams['debug'],
            "Outputs differ on line %d. Expected \"%s\" but was \"%s\".\n",
            $number_of_lines, $test_case_output_line, $program_output_line);
    }
}

// determine result (i.e. score and status)
$score = ($number_of_lines == 0) ? 100 : intval(100 * $number_of_correct_lines / $number_of_lines);
$status = ($score > 0) ? ACCEPTED : WRONG_ANSWER;

// set score
fwrite($streams['score'], $score);

// close streams
close_streams();

// exit with suitable status
exit($status);
