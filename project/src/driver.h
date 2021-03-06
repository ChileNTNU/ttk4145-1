// C++11    (c) 2014 Vladimír Štill <xstill@fi.muni.cz>

/** the middle level API for elevator */

#include "io.h"

#ifndef SRC_DRIVER_H
#define SRC_DRIVER_H

namespace elevator {

enum class ButtonType {
    CallUp = 0,
    CallDown = 1,
    TargetFloor = 2
};

struct Button {
    Button() = default;
    Button( ButtonType type, int floor ) : _type( type ), _floor( floor ) { }

    ButtonType type() { return _type; }
    int floor() { return _floor; }

  private:
    ButtonType _type;
    int _floor;
};

enum class Direction { Up, Down };

struct Driver {

    Driver();
    ~Driver();

    /* initialize the elevator -- disable all lights and run to lowest floor */
    void init();
    void shutdown();

    void setButtonLamp( Button button, bool state );
    void setStopLamp( bool state );
    void setDoorOpenLamp( bool state );
    void setFloorIndicator( int floor );

    bool getButtonLamp( Button button );
    bool getStopLamp();
    bool getDoorOpenLamp();
    int getFloorIndicator();

    void stopElevator();
    int getFloor();
    bool getStop();
    bool getObstruction();

    void goToFloor( int );
    void goUpToFloor( int );
    void goDownToFloor( int );
    void goToBottom();
    void goToTop();

  private:
    void _setMotorSpeed( Direction, int );
    void _movingOnFloor( int );
    void _goTo( Direction, int );

    int _minFloor;
    int _maxFloor;
    Direction _lastDirection;
    int _lastFloor;
    bool _moving;
    lowlevel::IO _lio;
};

}

#endif // SRC_DRIVER_H
