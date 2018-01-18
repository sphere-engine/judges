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

import java.io.*;
import java.util.*;
import java.lang.reflect.Constructor;

class Judge
{
    // file descriptors
    private final int TEST_CASE_INPUT_FD = 0;
    private final int TEST_CASE_OUTPUT_FD = 4;
    private final int PROGRAM_SOURCE_FD = 5;
    private final int PROGRAM_OUTPUT_FD = 3;
    private final int PROGRAM_INPUT_FD = 8; // interactive only
    private final int SCORE_FD = 1;
    private final int DEBUG_FD = 6;

    // statuses
    private final int ACCEPTED = 0;
    private final int WRONG_ANSWER = 1;

    // streams
    private Map<String, BufferedReader> input_streams = new HashMap<String, BufferedReader>();
    private Map<String, PrintWriter> output_streams = new HashMap<String, PrintWriter>();

    public static void main (String[] args) throws java.lang.Exception {
        Judge judge = new Judge();

        judge.initializeStreams();
        int status = judge.run();
        judge.closeStreams();

        // exit with suitable status
        System.exit(status);
    }

    ////////////////////////////////////////////////////////////////////////////

    public int run() throws java.lang.Exception {
        int number_of_lines = 0;
        int number_of_correct_lines = 0;

        for (String test_case_output_line; (test_case_output_line =
                input_streams.get("test_case_output").readLine()) != null; ) {
            String program_output_line = input_streams.get("program_output").readLine();
            number_of_lines++;

            if (program_output_line == null) {
                output_streams.get("debug").format(Locale.UK,
                    "Outputs differ on line %d. Expected \"%s\" but program's output has ended before.\n",
                    number_of_lines, test_case_output_line);
                continue;
            }

            if (test_case_output_line.equals(program_output_line)) {
                number_of_correct_lines++;
            } else {
                output_streams.get("debug").format(Locale.UK,
                    "Outputs differ on line %d. Expected \"%s\" but was \"%s\".\n",
                    number_of_lines, test_case_output_line, program_output_line);
            }
        }

        // determine result (i.e. score and status)
        int score = (number_of_lines == 0) ? 100 : 100 * number_of_correct_lines / number_of_lines;
        int status = (score > 0) ? ACCEPTED : WRONG_ANSWER;

        // set score
        output_streams.get("score").print(score);

        return status;
    }

    ////////////////////////////////////////////////////////////////////////////

    private void initializeStreams() throws java.lang.Exception {
        input_streams.put("test_case_input", getBufferedReaderByFileDescriptor(TEST_CASE_INPUT_FD));
        input_streams.put("test_case_output", getBufferedReaderByFileDescriptor(TEST_CASE_OUTPUT_FD));
        input_streams.put("program_source", getBufferedReaderByFileDescriptor(PROGRAM_SOURCE_FD));
        input_streams.put("program_output", getBufferedReaderByFileDescriptor(PROGRAM_OUTPUT_FD));

        output_streams.put("program_input", getPrintWriterByFileDescriptor(PROGRAM_INPUT_FD)); // interactive only
        output_streams.put("score", getPrintWriterByFileDescriptor(SCORE_FD));
        output_streams.put("debug", getPrintWriterByFileDescriptor(DEBUG_FD));
    }

    private void closeStreams() throws java.lang.Exception {
        for (Map.Entry<String, BufferedReader> entry : input_streams.entrySet()) {
            entry.getValue().close();
        }

        for (Map.Entry<String, PrintWriter> entry : output_streams.entrySet()) {
            entry.getValue().flush();
            entry.getValue().close();
        }
    }

    private FileDescriptor openFileByFileDescriptor(Integer n) throws java.lang.Exception {
        Constructor<FileDescriptor> ctor = FileDescriptor.class.getDeclaredConstructor(Integer.TYPE);
        ctor.setAccessible(true);
        FileDescriptor fd = ctor.newInstance(n);
        ctor.setAccessible(false);
        return fd;
    }

    private PrintWriter getPrintWriterByFileDescriptor(Integer n) throws java.lang.Exception {
        FileDescriptor stream_fd = openFileByFileDescriptor(n);
        return new PrintWriter(new FileWriter(stream_fd));
    }

    private BufferedReader getBufferedReaderByFileDescriptor(Integer n) throws java.lang.Exception {
        FileDescriptor stream_fd = openFileByFileDescriptor(n);
        return new BufferedReader(new FileReader(stream_fd));
    }
}
