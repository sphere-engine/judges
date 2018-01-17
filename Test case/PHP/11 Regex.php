<?php

// file streams
const TEST_CASE_INPUT_FD = 0;
const TEST_CASE_OUTPUT_FD = 4;
const PROGRAM_SOURCE_FD = 5;
const PROGRAM_OUTPUT_FD = 3;
const SCORE_FD = 1;
const P_INFO_FD = 6;
const U_INFO_FD = 7;

// results
const ACCEPTED = 0;
const WRONG_ANSWER = 1;
const INTERNAL_ERROR = 2;

function judge_init()
{
    global $handles;
    $handles['test_case_input'] = fopen('php://fd/' . TEST_CASE_INPUT_FD , "r");
    $handles['test_case_output'] = fopen('php://fd/' . TEST_CASE_OUTPUT_FD , "r");
    $handles['program_source'] = fopen('php://fd/' . PROGRAM_SOURCE_FD , "r");
    $handles['program_output'] = fopen('php://fd/' . PROGRAM_OUTPUT_FD , "r");
    $handles['score'] = fopen('php://fd/' . SCORE_FD , "w");
    $handles['p_info'] = fopen('php://fd/' . P_INFO_FD , "w");
    $handles['u_info'] = fopen('php://fd/' . U_INFO_FD , "w");
}

function judge_finalize()
{
    global $handles;
    foreach ($handles as $h) {
        fclose($h);
    }
}

judge_init();

$program_output = stream_get_contents($handles['program_output']);
$result = ACCEPTED;

while($pattern = trim(fgets($handles['test_case_output']))) {
    if (!preg_match($pattern, $program_output)) {
        $result = WRONG_ANSWER;
        break;
    }
}

judge_finalize();
exit($result);
