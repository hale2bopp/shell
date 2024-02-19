/* \file shellSignal.h
 * \brief Helper file for signal
 *
 */

#ifndef SHELL_SIGNAL_H
#define SHELL_SIGNAL_H

#include "signal.h"
void registerSigint(void);
void registerSigchld(void);
void signalHandler(int sig);
void handleSigchld(int sig);
void registerSignals(void);
bool getCoutAlarm(void);
#endif  // SHELL_SIGNAL_H
