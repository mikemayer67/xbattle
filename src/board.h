#ifndef _BOARD_H_
#define _BOARD_H_

class Board
{
  public:
    Board(void)
      : _width(0), _height(0)
    {}

    void init();

  private:

    int _width;
    int _height;

    vector<Cell>
    
    vector<XPoint> _size;
}

#endif // _BOARD_H_
