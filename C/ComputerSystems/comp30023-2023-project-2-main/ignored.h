
// https://stackoverflow.com/questions/23673174/how-to-ignore-new-changes-to-a-tracked-file-with-git

// git update-index --assume-unchanged ignored.h
// git update-index --no-assume-unchanged ignored.h

// contains stuff i want changes to ignored for
// so they can be locally and for pipeline testing different

#define DEBUG_CLOSE false  // close server and client for debugging to avoid port binding problems
#define DEBUG_PRINT false // print debug messages