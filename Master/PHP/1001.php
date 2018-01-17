<?php

// file streams
const TESTCASE_INFO_FD = 0;
const USER_SOURCE_FD = 5;
const SCORE_FD = 1;
const P_INFO_FD = 6;
const U_INFO_FD = 7;

function judge_init()
{
    global $handles;
    $handles['testcase_result'] = fopen('php://fd/' . TESTCASE_INFO_FD , "r");
    $handles['user_source'] = fopen('php://fd/' . USER_SOURCE_FD , "r");
    $handles['score'] = fopen('php://fd/' . SCORE_FD , "w");
    $handles['p_info'] = fopen('php://fd/' . P_INFO_FD , "w");
    $handles['u_info'] = fopen('php://fd/' . U_INFO_FD , "w");
}

function judge_finalize()
{
    global $handles;
    foreach ($handles as $h)
        fclose($h);
}

judge_init();
$final_memory = 0;
$final_time = 0;
$final_score = 0;
$tc_number = 0;
$tc_statuses = [];

while ($line = fscanf($handles['testcase_result'], "%d %3s %lf %d %lf %d\n"))
{
    list ($test_number, $status, $score, $signal, $time, $memory) = $line;
    fprintf($handles['p_info'], "test %d - %s (score=%lf, sig=%d, time=%lf, mem=%d)\n", $test_number, $status, $score, $signal, $time, $memory);
    if ($status == "AC") {
        if ($memory > $final_memory) $final_memory = $memory;
        $final_time += $time;
        $final_score++;
    }
    if (!isset($tc_statuses[$status])) $tc_statuses[$status] = 0;
    $tc_statuses[$status]++;
    $tc_number++;
}

if (isset($tc_statuses["RE"]) && $tc_statuses["RE"] == $tc_number) 
    $final_status = "RE";
elseif (isset($tc_statuses["TLE"]) && $tc_statuses["TLE"] == $tc_number) 
    $final_status = "TLE";
elseif (!isset($tc_statuses["AC"])) 
    $final_status = "WA";
else
    $final_status = "AC";

fprintf($handles['score'], "%s %.2lf 0 %lf %d\n", $final_status, (100*$final_score / $tc_number), $final_time, $final_memory);
judge_finalize();
