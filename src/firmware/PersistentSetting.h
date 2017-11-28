#pragma once

#include <MutilaDebug.h>
#include <stdint.h>
#include <EEPROM.h>

template <class T>
class PersistentSetting {
public:
    /*! Constructor
     * \param eepromOffset the offset in EEPROM where this setting starts (may be multi-byte)
     * \param min the minimum value for this setting
     * \param max the maximum value for this setting
     * \param def the default value for this setting
     */
    PersistentSetting(uint16_t eepromOffset, T min, T max, T def) : 
        _eepromOffset(eepromOffset),
        _min(min),
        _max(max),
        _def(def)
    {
        this->load();
    }

    /*! Load value from EEPROM
     * \return the loaded value
     */
    T load() {
        uint8_t* ptr = (uint8_t*)(&_value);
        for (uint8_t i=0; i<sizeof(T); i++) {
            ptr[i] = EEPROM.read(_eepromOffset+i);
        }
        if (_value < _min || _value > _max) {
            _value = _def;
        }
        return _value;
    }

    /*! Save the value to EEPROM
     *  Note: we're using the EEPROM.update() call, so EEPROM is only actually
     *  written if the value is different from the value currently in EEPROM. 
     *  This measure is an attempt to reduce wear on the EEPROM
     */
    void save()
    {
        uint8_t* ptr = (uint8_t*)(&_value);
        DB(F("EEPROM write at "));
        DB(_eepromOffset);
        for (uint8_t i=0; i<sizeof(T); i++) {
            DB('+');
            DB(i);
            EEPROM.update(_eepromOffset+i, ptr[i]);
        }
        DBLN('.');
    }

    /*! Get the current value of the setting
     */
    T get() { return _value; }

    /*! Sets the in-RAM value of the setting.
     *  \param v the value to be set. If v is less than the minimum value or 
     *         greater than the maximum value, no change will be made.
     *  Note: this function does NOT save the new value to EEPROM. To do that, 
     *  save() must be called.
     */
    T operator=(T v) { if (v >= _min && v<=_max) { _value = v; } return _value; }

    /*! Sets the in-RAM value of the setting.
     *  \param v the value to be set. If v is less than the minimum value or 
     *         greater than the maximum value, no change will be made.
     *  \return true if the value was set successfully, false otherwise (invalid v)
     *  Note: this function does NOT save the new value to EEPROM. To do that, 
     *  save() must be called.
     */
    bool set(T v) { if (v >= _min && v<=_max) { _value = v; return true; } else { return false; } }

    /*! Get the size in bytes of the setting in EEPROM.
     */
    size_t size() { return sizeof(T); }

private:
    uint16_t _eepromOffset;

protected:
    T _value;
    T _min;
    T _max;
    T _def;
};

