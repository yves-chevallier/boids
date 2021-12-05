#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class ExtendedColor : public sf::Color
{
   protected:
    static double hue2rgb(double p, double q, double t);

   public:
    using sf::Color::Color;

    ExtendedColor(sf::Color& color);

    static ExtendedColor fromHex(const std::string& hexcolor);
    static ExtendedColor fromHSL(double h, double s, double l);
    static ExtendedColor fromHSV(double h, double s, double v);

    double min() const;
    double max() const;  

    double getHue() const;
    double getSaturation() const;
    double getLuminance() const;

    void setHue(double hue);
    void setSaturation(double saturation);
    void setLuminance(double luminance);

    //sf::Color operator=(const ExtendedColor& color);

    std::string toHex() const;
};




