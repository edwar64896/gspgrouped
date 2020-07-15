#ifndef GSP_GROUPED
#define GSP_GROUPED

#include <Arduino.h>

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

        //static void _isr_startCheckAll(gspGrouped * pInstance);
        static void _ISR();

        virtual void setFirstInstance(gspGrouped *)=0;
        virtual gspGrouped * getFirstInstance()=0;

        uint8_t getInstanceId() {return instanceId;}
        void setInstanceId(uint8_t _instanceId) {instanceId=_instanceId;}
        static int register_instance(gspGrouped * newInstance);


        // stream management functions
        static void setup(Stream& _stream = Serial) {gspGrouped::gspStream = _stream;}
        static Stream & gspStream;
        static char getChar();
        static void setInitialInstance(gspGrouped * instance) { gspGrouped::_interruptFirstInstance = instance;}

        static void startTimer();
        static void setTimerCap(uint32_t tcap) {gspGrouped::_timerCap = tcap;}

    protected:
        // switch list management functions

        void setNextInstance(gspGrouped * nextInstance);
        gspGrouped * getNextInstance();
        void backToStart();
        static bool _isr_checking;

        static gspGrouped * _interruptFirstInstance; // specifically for interrupt driven checker.

    private:

        static uint32_t _tmrCtr;
        static uint32_t _timerCap;

        gspGrouped * nextInstance = nullptr; //manual checker

        uint8_t instanceId=0;
};



#endif