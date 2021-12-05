
#include "color.hpp"
#include "hsl.hpp"

#include <iomanip>

double ExtendedColor::hue2rgb(double p, double q, double t) {
    if (t < 0) t += 1;
    if (t > 1) t -= 1;
    if (t < 1.0 / 6.0) return p + (q - p) * 6.0 * t;
    if (t < 1.0 / 2.0) return q;
    if (t < 2.0 / 3.0) return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
    return p;
}

ExtendedColor::ExtendedColor(sf::Color& color)
{
    this->r = color.r;
    this->g = color.g;
    this->b = color.b;
}

ExtendedColor fromHex(const std::string& hexcolor)
{
    sf::Color color = sf::Color::Black;
    if (hexcolor.size() == 7)  // #ffffff
    {
        color.r = strtoul(hexcolor.substr(1, 2).c_str(), NULL, 16);
        color.g = strtoul(hexcolor.substr(3, 2).c_str(), NULL, 16);
        color.b = strtoul(hexcolor.substr(5, 2).c_str(), NULL, 16);
    } else if (hexcolor.size() == 4)  // #fff
    {
        color.r = strtoul(hexcolor.substr(1, 1).c_str(), NULL, 16) * 17;
        color.g = strtoul(hexcolor.substr(2, 1).c_str(), NULL, 16) * 17;
        color.b = strtoul(hexcolor.substr(3, 1).c_str(), NULL, 16) * 17;
    }
    return ExtendedColor(color);
}

ExtendedColor ExtendedColor::fromHSL(double h, double s, double l)
{
    double r, g, b;
    if (s == 0) {
        r = g = b = l;  // achromatic
    } else {
        double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        double p = 2 * l - q;
        r = hue2rgb(p, q, h + 1.0f / 3.0f);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1.0f / 3.0f);
    }
    return ExtendedColor(r * 255, g * 255, b * 255);
}

ExtendedColor ExtendedColor::fromHSV(double h, double s, double v)
{
    double r, g, b;
    if (s == 0) {
        r = g = b = v;  // achromatic
    } else {
        double hh = h;
        if (hh >= 360) hh = 0;
        hh /= 60;
        int i = (int)hh;
        double ff = hh - i;
        double p = v * (1 - s);
        double q = v * (1 - s * ff);
        double t = v * (1 - s * (1 - ff));
        switch (i) {
            case 0:
                r = v;
                g = t;
                b = p;
                break;
            case 1:
                r = q;
                g = v;
                b = p;
                break;
            case 2:
                r = p;
                g = v;
                b = t;
                break;
            case 3:
                r = p;
                g = q;
                b = v;
                break;
            case 4:
                r = t;
                g = p;
                b = v;
                break;
            default:
                r = v;
                g = p;
                b = q;
                break;
        }
    }
    return ExtendedColor(r * 255, g * 255, b * 255);
}

double ExtendedColor::min() const
{
    double min_value = this->r;
    min_value = this->g < min_value ? this->g : min_value;
    min_value = this->b < min_value ? this->b : min_value;
    return min_value;
}

double ExtendedColor::max() const
{
    double max_value = this->r;
    max_value = this->g > max_value ? this->g : max_value;
    max_value = this->b > max_value ? this->b : max_value;
    return max_value;
}

std::string ExtendedColor::toHex() const
{
    std::stringstream ss;
    ss << "#" << std::hex << std::setfill('0') << std::setw(2) << this->r
       << std::setw(2) << this->g << std::setw(2) << this->b;
    return ss.str();
}

double ExtendedColor::getHue() const
{
    auto hsl = HSL::fromRGB(*this);
    return hsl.hue;
}

double ExtendedColor::getSaturation() const
{
    auto hsl = HSL::fromRGB(*this);
    return hsl.saturation;
}

double ExtendedColor::getLuminance() const
{
    auto hsl = HSL::fromRGB(*this);
    return hsl.luminance;
}

void ExtendedColor::setHue(double hue) 
{
    auto hsl = HSL::fromRGB(*this);
    hsl.hue = hue;
    sf::Color color = hsl.toRGB();
    this->r = color.r;
    this->g = color.g;
    this->b = color.b;
}

void ExtendedColor::setSaturation(double saturation)
{
    auto hsl = HSL::fromRGB(*this);
    hsl.saturation = saturation;
    sf::Color color = hsl.toRGB();
    this->r = color.r;
    this->g = color.g;
    this->b = color.b;
}

void ExtendedColor::setLuminance(double luminance)
{
    auto hsl = HSL::fromRGB(*this);
    hsl.luminance = luminance;
    sf::Color color = hsl.toRGB();
    this->r = color.r;
    this->g = color.g;
    this->b = color.b;
}
