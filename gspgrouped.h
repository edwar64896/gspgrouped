#ifndef GSP_GROUPED
#define GSP_GROUPED

#include "Arduino.h"

class gspGrouped {

    public:

        gspGrouped();
        ~gspGrouped();

        static void checkAll(gspGrouped* firstInstance);
        static void resetAll(gspGrouped* firstInstance);
        static void  _isrAll(gspGrouped * firstInstance);

        virtual bool check(){return true;};
        virtual void reset(){};
        virtual bool _isr(){return true;}

        static void _isr_startCheckAll(gspGrouped * pInstance);
        static void _ISR();

        virtual void setFirstInstance(gspGrouped *)=0;
        virtual gspGrouped * getFirstInstance()=0;

        uint8_t getInstanceId() {return instanceId;}
        void setInstanceId(uint8_t _instanceId) {instanceId=_instanceId;}
        static int register_instance(gspGrouped * newInstance);

    protected:
        // switch list management functions

        void setNextInstance(gspGrouped * nextInstance);
        gspGrouped * getNextInstance();
        void backToStart();

    private:

        static gspGrouped * _interruptFirstInstance; // specifically for interrupt driven checker.

        gspGrouped * nextInstance = nullptr; //manual checker

        uint8_t instanceId=0;
};



#endif