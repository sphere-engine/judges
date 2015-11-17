<?php

// return values
const SPOJ_RV_AC = 0;
const SPOJ_RV_WA = 1;
const SPOJ_RV_SE = 3;
const SPOJ_RV_RE = 4;
const SPOJ_RV_TLE = 5;
const SPOJ_RV_MLE = 6;
const SPOJ_RV_EOF = 7;
const SPOJ_RV_IE = 255;

// file streams
const SPOJ_P_IN_FD = 0;
const SPOJ_P_OUT_FD = 4;
const SPOJ_T_OUT_FD = 3;
const SPOJ_T_SRC_FD = 5;
const SPOJ_SCORE_FD = 1;
const SPOJ_P_INFO_FD = 6;
const SPOJ_U_INFO_FD = 7;

function spoj_init()
{
    global $spoj_p_in, $spoj_p_out, $spoj_t_out, $spoj_t_src, $spoj_score, $spoj_u_info, $spoj_p_info;
    $spoj_p_in = fopen('php://fd/' . SPOJ_P_IN_FD , "r");
    $spoj_p_out = fopen('php://fd/' . SPOJ_P_OUT_FD , "r");
    $spoj_t_out = fopen('php://fd/' . SPOJ_T_OUT_FD , "r");
    $spoj_t_src = fopen('php://fd/' . SPOJ_T_SRC_FD , "r");
    $spoj_score = fopen('php://fd/' . SPOJ_SCORE_FD , "w");
    $spoj_p_info = fopen('php://fd/' . SPOJ_P_INFO_FD , "w");
    $spoj_u_info = fopen('php://fd/' . SPOJ_U_INFO_FD , "w");
    //$spoj_for_tested = fopen('php://fd/' . SPOJ_FOR_TESTED_FD , "w");
}

function spoj_finalize()
{
    global $spoj_p_in, $spoj_p_out, $spoj_t_out, $spoj_t_src, $spoj_score, $spoj_u_info, $spoj_p_info;
    foreach ([$spoj_p_in, $spoj_p_out, $spoj_t_out, $spoj_t_src, $spoj_score, $spoj_u_info, $spoj_p_info] as $fd)
        fclose($fd);
}

spoj_init();
$final_memory = 0;
$final_time = 0;
$final_score = 0;
$tc_number = 0;
$tc_statuses = [];

while ($line = fscanf($spoj_p_in, "%d %3s %lf %d %lf %d\n"))
{
    list ($test_number, $status, $score, $signal, $time, $memory) = $line;
    fprintf($spoj_p_info, "test %d - %s (score=%lf, sig=%d, time=%lf, mem=%d)\n", $test_number, $status, $score, $signal, $time, $memory);
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
elseif (isset($tc_statuses["AC"]) && $tc_statuses["AC"] == 0) 
    $final_status = "WA";
else
    $final_status = "AC";

fprintf($spoj_score, "%s %.2lf 0 %lf %d\n", $final_status, (100*$final_score / $tc_number), $final_time, $final_memory);
spoj_finalize();

if ($final_status == "AC") 
    return SPOJ_RV_AC;
elseif ($final_status == "WA") 
    return SPOJ_RV_WA;
elseif ($final_status == "TLE") 
    return SPOJ_RV_TLE;
elseif ($final_status == "RE") 
    return SPOJ_RV_RE;
else
    return SPOJ_RV_IE;