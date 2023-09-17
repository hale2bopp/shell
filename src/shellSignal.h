/* \file shellSignal.h
 * \brief Helper file for signal
 *
 */

#ifndef SHELL_SIGNAL_H
#define SHELL_SIGNAL_H

#include "signal.h"
#include <sys/wait.h>
void signalHandler(int sig);
void handleSigchld(int sig);
void registerSignals(void);

#endif  // SHELL_SIGNAL_H
