/*
Execute another application, piping input to and from its stdin and stdout.
*/

#ifndef __PIPE_H__
#define __PIPE_H__

typedef pair<FILE *, FILE *> FILEPair;

// Return input and output file pointers.
// User is responsible for closing them.
// May have to close out before reading from in.
FILEPair create_pipe(char *const cmd[]) {
  int p2c_fds[2], c2p_fds[2];

  assert(pipe(p2c_fds) == 0);
  assert(pipe(c2p_fds) == 0);

  int pid = fork();
  assert(pid != -1);
  if(pid != 0) { // parent
    close(p2c_fds[0]);
    close(c2p_fds[1]);

    FILE *in = fdopen(c2p_fds[0], "r");
    FILE *out = fdopen(p2c_fds[1], "w");

    assert(in && out);

    return FILEPair(in, out);
  }
  else { // child
    close(p2c_fds[1]);
    close(c2p_fds[0]);

    assert(dup2(p2c_fds[0], fileno(stdin)) != -1);
    assert(dup2(c2p_fds[1], fileno(stdout)) != -1);
    execvp(cmd[0], cmd);

    // Execution should not reach here.
    assert(0);
    return FILEPair(NULL, NULL);
  }
}

#endif
