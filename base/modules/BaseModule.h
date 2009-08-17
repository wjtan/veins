/* -*- mode:c++ -*- ********************************************************
 * file:        BaseModule.h
 *
 * author:      Steffen Sroka
 *              Andreas Koepke
 *
 * copyright:   (C) 2004 Telecommunication Networks Group (TKN) at
 *              Technische Universitaet Berlin, Germany.
 *
 *              This program is free software; you can redistribute it
 *              and/or modify it under the terms of the GNU General Public
 *              License as published by the Free Software Foundation; either
 *              version 2 of the License, or (at your option) any later
 *              version.
 *              For further information see file COPYING
 *              in the top level directory
 ***************************************************************************
 * part of:     framework implementation developed by tkn
 **************************************************************************/


#ifndef BASE_MODULE_H
#define BASE_MODULE_H

#include <sstream>
#include <omnetpp.h>

#include "ImNotifiable.h"
#include "FindModule.h"
#include "BaseBattery.h"


#define EV_clear (ev.isDisabled()||!debug) ? ev : ev
//#define EV (ev.isDisabled()||!debug) ? ev : ev << logName() << "::" << getClassName() << ": " ==> EV is now part of <omnetpp.h>

#ifndef coreEV
#define coreEV_clear (ev.isDisabled()||!coreDebug) ? ev : ev
#define coreEV (ev.isDisabled()||!coreDebug) ? ev : ev << logName() << "::" << getClassName() <<": "
#endif


class BaseUtility;

/**
 * @brief Base class for all simple modules of a host that want to have
 * access to the Blackboard module.
 *
 * The basic module additionally provides a function findHost which
 * returns a pointer to the host module and a function hostIndex to
 * return the index of the host module. The latter one correspondes to
 * the index shown in tkenv and comes in very handy for testing and
 * debugging using tkenv. It is used e.g. in all the 'print' macros
 * used for debugging.
 *
 * There is no Define_Module() for this class because we use
 * BasicModule only as a base class to derive all other
 * module. There will never be a stand-alone BasicModule module
 * (and that is why there is no Define_Module() and no .ned file for
 * BasicModule).
 *
 * @see Blackboard
 *
 * @ingroup baseModules
 *
 * @author Steffen Sroka
 * @author Andreas Koepke
 */
class BaseModule: public cSimpleModule, public ImNotifiable {
  protected:
    /** @brief Cached pointer to the utility module*/
    BaseUtility *utility;

    /** @brief Debug switch for all other modules*/
    bool debug;

    BaseBattery* battery;
	int deviceID;

  protected:
    /** @brief Function to get a pointer to the host module*/
    cModule *findHost(void);

    /** @brief Function to get the logging name of id*/
    std::string getLogName(int);


    void registerWithBattery(const std::string& name, int numAccounts);

	void drain(DrainAmount& amount, int activity);
	void drainCurrent(double amount, int activity);
	void drainEnergy(double amount, int activity);

  public:
    BaseModule();

    /** @brief Basic initialization for all modules */
    virtual void initialize(int);

    /**
     * @brief Divide initialization into two stages
     *
     * In the first stage (stage==0), modules subscribe to notification
     * categories at Blackboard. The first notifications
     * (e.g. about the initial values of some variables such as RadioState)
     * should take place earliest in the second stage (stage==1),
     * when everyone interested in them has already subscribed.
     */
    virtual int numInitStages() const {
    	return 2;
    }

    /**
     * @brief Function to get the logging name of the host
     *
     * The logging name is the ned module name of the host (unless the
     * host ned variable loggingName is specified). It can be used for
     * logging messages to simplify debugging in TKEnv.
     */
    std::string logName(void);

    /**
     * @brief Get a reference to the local node module
     */
    cModule * getNode(){
    	return findHost();
    };

    /**
     * @brief Called by the Blackboard whenever a change of a category occurs
     * to which we have subscribed. Redefined from ImNotifiable.
     * In this base class just handle the context switching and
     * some debug notifications
     */
    virtual void receiveBBItem(int category, const BBItem *details, int scopeModuleId) {
        Enter_Method_Silent();
    }
};

#endif
