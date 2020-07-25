#ifndef GSP_GROUPED
#define GSP_GROUPED

#include <Arduino.h>

#define gspGrouped_FLASHSTATE_CAP 64

class gspGrouped {

    public:

        gspGrouped(){};
        ~gspGrouped(){};

        static void checkAll(gspGrouped* firstInstance);
        static void resetAll(gspGrouped* firstInstance);
        static void  _isrAll(gspGrouped * firstInstance);

        virtual bool check(){return true;};
        virtual void reset(){};
        virtual bool _isr(){return true;}

        //static void _isr_startCheckAll(gspGrouped * pInstance);
        static void _ISR();
        static void _ISR_fast();
        static void _ISR_slow();

        virtual void setFirstInstance(gspGrouped *)=0;
        virtual gspGrouped * getFirstInstance()=0;

        uint8_t getInstanceId() {return instanceId;}
        void setInstanceId(uint8_t _instanceId) {instanceId=_instanceId;}
        static int register_instance(gspGrouped * newInstance);


        // stream management functions
        static void setup(Stream& _stream = Serial) {gspGrouped::gspStream = _stream;}
        static Stream & gspStream;
        static char getChar();
        static void setInitialInstance (gspGrouped * instance) { gspGrouped::_isr_setInitialInstance (instance);}
        static void startTimer() {_isr_startTimer();}
        static void _isr_setInitialInstance     (gspGrouped * instance) { gspGrouped::_interruptFirstInstance       = instance;}
        static void _isr_setInitialInstance_fast(gspGrouped * instance) { gspGrouped::_interruptFirstInstance_fast  = instance;}
        static void _isr_setInitialInstance_slow(gspGrouped * instance) { gspGrouped::_interruptFirstInstance_slow  = instance;}
        static void _isr_startTimer() {	TIMSK2 |= (1 << TOIE2); }

        volatile static bool        _flashStateFast;
        volatile static bool        _flashStateSlow;
        volatile static bool        _flashState;
        volatile static uint32_t    _flashStateC1;

    protected:
        // switch list management functions

        void setNextInstance(gspGrouped * nextInstance);
        gspGrouped * getNextInstance();
        void backToStart();
        static bool _isr_checking;

        static gspGrouped * _interruptFirstInstance; // specifically for standard interrupt driven checker.
        static gspGrouped * _interruptFirstInstance_fast; // specifically for fast interrupt driven checker.
        static gspGrouped * _interruptFirstInstance_slow; // specifically for slow interrupt driven checker.

    private:

        gspGrouped * nextInstance = nullptr; //manual checker

        uint8_t instanceId=0;
};



#endif