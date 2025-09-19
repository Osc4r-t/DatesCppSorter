#include <iostream>
using namespace std;

class Order {
private:
    int sec, min, hr, day, month, n;
    string restaurant, name;
public:
    Order() : sec(0), min(0), hr(0), day(0), month(0), n(0), restaurant(""), name("") {}
    Order(int _s, int _min, int _h, int _d, int _m, int _n, const string& _r, const string& _nm)
       : sec(_s), min(_min), hr(_h), day(_d), month(_m), n(_n),
         restaurant(_r), name(_nm) {}
    Order(int _s, int _min, int _h, int _d, int _m)
        : sec(_s), min(_min), hr(_h), day(_d), month(_m),
          n(0), restaurant(""), name("") {}

    int getSec() const { return sec; }
    int getMin() const { return min; }
    int getHour() const { return hr; }
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getNumber() const { return n; }
    string getName() const { return name; }
    string getRestaurant() const { return restaurant; }

    int compareTime(const Order& other) const {

        if (month != other.month) {return (month < other.month) ? (-2) : (2);}
        if (day   != other.day)   {return (day   < other.day)   ? (-3) : (3);}
        if (hr    != other.hr)    {return (hr    < other.hr)    ? (-4) : (4);}
        if (min   != other.min)   {return (min   < other.min)   ? (-5) : (5);}
        if (sec   != other.sec)   {return (sec   < other.sec)   ? (-6) : (6);}
        return 0;
    }

    bool operator == (const Order& other) const {
        return (compareTime(other) == 0);
    }
    bool operator > (const Order& other) const {
        return (compareTime(other) > 0);
    }
    bool operator < (const Order& other) const {
        return (compareTime(other) < 0);
    }
    bool operator >= (const Order& other) const {
        return (compareTime(other) >= 0);
    }
    bool operator <= (const Order& other) const {
        return (compareTime(other) <= 0);
    }

    friend ostream& operator<<(ostream& os, const Order& order){
      string mes;
      if (order.getMonth() == 1) mes = "ENE";
      if (order.getMonth() == 2) mes = "FEB";
      if (order.getMonth() == 3) mes = "MAR";
      if (order.getMonth() == 4) mes = "ABR";
      if (order.getMonth() == 5) mes = "MAY";
      if (order.getMonth() == 6) mes = "JUN";
      if (order.getMonth() == 7) mes = "JUL";
      if (order.getMonth() == 8) mes = "AGO";
      if (order.getMonth() == 9) mes = "SEP";
      if (order.getMonth() == 10) mes = "OCT";
      if (order.getMonth() == 11) mes = "NOV";
      if (order.getMonth() == 12) mes = "DIC";

      os << mes << " " << order.getDay() << " " << order.getHour() << ":" << order.getMin() << ":" << order.getSec()
        << " R:" << order.getRestaurant() << "O:" << order.getName() << "(" << order.getNumber() << ")";

      return os;
    }

};
