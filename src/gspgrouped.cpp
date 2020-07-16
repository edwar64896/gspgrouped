#include "gspgrouped.h"

//static
gspGrouped * gspGrouped::_interruptFirstInstance=nullptr;

//static
Stream & gspGrouped::gspStream=Serial;

bool gspGrouped::_isr_checking = false;
volatile bool gspGrouped::_flashState = false;
volatile uint32_t gspGrouped::_flashStateC1 = 0;

gspGrouped::gspGrouped() {

}

gspGrouped::~gspGrouped() {}

void gspGrouped::startTimer() {
	TIMSK2 |= (1 << TOIE2);
}
//static
void gspGrouped::_ISR() {
	if (gspGrouped::_isr_checking) {
		if (gspGrouped::_interruptFirstInstance != nullptr) {
			gspGrouped::_isrAll(gspGrouped::_interruptFirstInstance);
		}
	}
}

// static method to register this switch 
int gspGrouped::register_instance(gspGrouped * newInstance) {

	int ctr=0;

	// going to use pInstance to walk the list
	gspGrouped* pInstance=nullptr;

	// if there are no instances registered at all
	// we set this to the first one then leave.
	if (newInstance->getFirstInstance()==nullptr) {
		newInstance->setFirstInstance(newInstance) ;
		newInstance->setInstanceId(ctr);

		return ctr;

	} else {

		// return a pointer to the first switch in the list.
		pInstance = newInstance->getFirstInstance();

	}

	// set up a loop to walk the list.
	while (1) {

		// increment the counter
		++ctr;

		// is there anything more in the list?
		if (pInstance->getNextInstance() == nullptr) {

			//register this new switch with the previous 
			//last switch in the list.
			//
			// note that the "nextSwitch" pointer in "*newSwitch" will be nullptr
			// as it is the last in the list now.
			pInstance->setNextInstance(newInstance);
			newInstance->instanceId=ctr;

			//Serial.println("anotherinstance");

			return ctr;


		} else {
			// if yes, set pInstance to the next switch in the list
			// and loop around. We are looking for the last one in the list.
			pInstance=pInstance->getNextInstance();

		}
	}
}

// store a pointer to the next switch in the list
// in this instance.
// if this is the last switch in the list, 
// nextSwitch will be nullptr.
void gspGrouped::setNextInstance(gspGrouped * nextInstance) {
	this->nextInstance=nextInstance;
}

// get the next switch in the list
gspGrouped * gspGrouped::getNextInstance() {
	return this->nextInstance;
}

//static method to go check all switch instances (called from loop function)
void gspGrouped::checkAll(gspGrouped * pInstance) {

	// loop while pInstance is not null
	while (1) {

		// if indeed there are no more switches to check,
		// we are done, otherwise we check the instance.
		if (pInstance==nullptr) 
			//we are done
			return;
		else
			// check the instance
			if (!pInstance->check())
				break;

		// set pSwitch to the next instance in the list
		// if there are no more instance, this will be set to nullptr
		pInstance=pInstance->getNextInstance();
	}
}

//static method to go check all instances (called from loop function)
void gspGrouped::_isrAll(gspGrouped * pInstance) {

	// loop while pInstance is not null
	while (1) {

		// if indeed there are no more instances to check,
		// we are done, otherwise we check the instance.
		if (pInstance==nullptr) 
			//we are done
			return;
		else
			// check the instance
			if (!pInstance->_isr())
				break;

		// set pInstance to the next switch in the list
		// if there are no more instances, this will be set to nullptr
		pInstance=pInstance->getNextInstance();
	}
}


//static method to go check all switch instances (called from loop function)
void gspGrouped::resetAll(gspGrouped * pInstance) {

	// loop while pSwitch is not null
	while (1) {

		// if indeed there are no more switches to check,
		// we are done, otherwise we check the instance.
		if (pInstance==nullptr) 
			//we are done
			return;
		else
			// reset the switch instance
			pInstance->reset();
		
		// set pSwitch to the next switch in the list
		// if there are no more switches, this will be set to nullptr
		pInstance=pInstance->getNextInstance();
	}
}

char gspGrouped::getChar()// Get a character from the Net buffer
{
  while (!gspStream.available()); // wait for data
  return ((char)gspStream.read()); // Thanks Doug
}


ISR(TIMER2_OVF_vect) {

    gspGrouped::_ISR();

	if (!(++gspGrouped::_flashStateC1 % gspGrouped_FLASHSTATE_CAP))
	gspGrouped::_flashState = !gspGrouped::_flashState;

}

