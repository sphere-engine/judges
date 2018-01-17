// Test case judge "Match lines" for Sphere Engine Problems
//
//
// The judge iterates over lines of the test case output file and compares them
// with corresponding lines from the program's output file.
//
// A submission is considered as correct (i.e. "accepted") if there is at least
// a single matching between corresponding lines (i.e. lines are identical).
//
// The score is calculated as a rounded percentage value of matched lines.
//
// EXAMPLE
// Test case output file:      Program's output
// 1                           1
// 2                           4
// 3                           3
// 4                           2
// 5                           5
//
// Status: accepted
// Score: 60 (out of 100)
//
// @copyright  Copyright (c) 2018 Sphere Research Labs (http://sphere-research.com)

package main

import (
    "bufio"
    "fmt"
    "os"
    "strconv"
)

// file descriptors
const TEST_CASE_INPUT_FD = 0
const TEST_CASE_OUTPUT_FD = 4
const PROGRAM_SOURCE_FD = 5
const PROGRAM_OUTPUT_FD = 3
const PROGRAM_INPUT_FD = 8 // interactive only
const SCORE_FD = 1
const DEBUG_FD = 6

// statuses
const ACCEPTED = 0
const WRONG_ANSWER = 1

var (
    inputStreams = make(map[string]*bufio.Reader)
    outputStreams = make(map[string]*os.File)
)

func initializeStreams(){
    inputStreams["test_case_input"] = bufio.NewReader(os.NewFile(TEST_CASE_INPUT_FD, "test_case_input"))
    inputStreams["test_case_output"] = bufio.NewReader(os.NewFile(TEST_CASE_OUTPUT_FD, "test_case_output"))
    inputStreams["program_source"] = bufio.NewReader(os.NewFile(PROGRAM_SOURCE_FD, "program_source"))
    inputStreams["program_output"] = bufio.NewReader(os.NewFile(PROGRAM_OUTPUT_FD, "program_output"))

    outputStreams["program_input"] = os.NewFile(PROGRAM_INPUT_FD, "program_input") // interactive only
    outputStreams["score"] = os.NewFile(SCORE_FD, "score")
    outputStreams["debug"] = os.NewFile(DEBUG_FD, "debug")
}

func closeStreams(){
    for _, v := range outputStreams {
        v.Close()
    }
}

func main(){
    initializeStreams()
    var status = runJudge()
    closeStreams()

    // exit with suitable status
    os.Exit(status)
}

////////////////////////////////////////////////////////////////////////////////

func runJudge() int {
    var numberOfLines = 0
    var numberOfCorrectLines = 0
    
    for {
        testCaseOutputLine, _, errTCO := inputStreams["test_case_output"].ReadLine()
        programOutputLine, _, errPO := inputStreams["program_output"].ReadLine()

        // finish when "test_case_output" stream is empty
        if errTCO != nil {
            break
        }

        numberOfLines++

        if errPO != nil {
            outputStreams["debug"].WriteString(fmt.Sprintf(
                "Outputs differ on line %d. Expected \"%s\" but program's output has ended before.\n",
                numberOfLines, testCaseOutputLine))
            continue
        }

        if string(testCaseOutputLine) == string(programOutputLine) {
            numberOfCorrectLines++
        } else {
            outputStreams["debug"].WriteString(fmt.Sprintf(
                "Outputs differ on line %d. Expected \"%s\" but was \"%s\".\n",
                numberOfLines, testCaseOutputLine, programOutputLine))
        }
    }

    // determine result (i.e. score and status)
    var score int
    
    if numberOfLines == 0 {
        score = 100
    } else {
        score = 100 * numberOfCorrectLines / numberOfLines;
    }

    var status int

    if score > 0 {
        status = ACCEPTED
    } else {
        status = WRONG_ANSWER
    }

    // set score
    outputStreams["score"].WriteString(strconv.Itoa(score))

    return status
}
