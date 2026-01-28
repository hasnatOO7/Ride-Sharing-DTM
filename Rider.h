#ifndef RIDER_H
#define RIDER_H

#include <string>

class Rider {
private:
    int id;
    std::string name;
    std::string location;

public:
    Rider(int id, const std::string& name, const std::string& location);

    int getId() const;
    const std::string& getName() const;
    const std::string& getLocation() const;

    void setLocation(const std::string& newLocation);
};

#endif // RIDER_H
