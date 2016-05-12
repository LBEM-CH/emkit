/* 
 * This file is a part of emkit.
 * 
 * emkit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any 
 * later version.
 * 
 * emkit is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public 
 * License for more details <http://www.gnu.org/licenses/>
 * 
 * Author:
 * Nikhil Biyani: nikhil(dot)biyani(at)gmail(dot)com
 * 
 */

#ifndef COMPLEX2DX_HPP
#define COMPLEX2DX_HPP

#include <iostream>
#include <complex>

namespace em {
    
    namespace element {

        /**
         * A class used to store complex data in 2dx.
         * Internally stores data in real and imaginary double values.
         * Also provides handy methods to directly set/get amplitudes and
         * phases.
         */
        template<typename ValueType_>
        class Complex {
        public:

            typedef ValueType_ value_type;
            typedef ValueType_& reference;

            /**
             * Default constructor which initializes to 0 + 0i 
             */
            Complex()
            : complex_(0, 0) {

            };

            /**
             * Constructor with real and imag part
             * @param real
             * @param imag
             */
            Complex(value_type real, value_type imag)
            : complex_(real, imag) {

            };

            /**
             * Operator overloading of = operator.
             * Equates the real and imag part of rhs to this object
             * @param rhs
             * @return equated instance
             */
            Complex& operator=(const Complex& rhs) {
                complex_ = rhs.complex_;
                return *this;
            };

            /**
             * Operator overloading of + operator.
             * Adds up the real and imag part of this object and rhs.
             * @param rhs
             * @return modified this object
             */
            Complex operator+(const Complex& rhs) const {
                return complex_ + rhs.complex_;
            };

            /**
             * Declaration of multiplication of a double with complex2dx
             * @param factor
             * @return 
             */
            template<typename ArithmeticType_>
            Complex operator*(ArithmeticType_ factor) const {
                static_assert(std::is_arithmetic<ArithmeticType_>::value, "The rhs should be of arithmetic type.");
                return Complex(real() * factor, imag() * factor);
            };

            template<typename ArithmeticType_>
            Complex& operator*=(ArithmeticType_ factor) const {
                *this = (*this) * factor;
                return *this;
            };

            /**
             * Declaration of multiplication of a Complex with another Complex
             * @param another complex
             * @return multiplied complex
             */
            Complex operator*(const Complex& other) const {
                return complex_ * other.complex_;
            };

            /**
             * Operator overloading of < operator
             * Amplitudes (aka abs) is used to check the condition.
             * @param rhs
             * @return result of this < rhs? 
             */
            bool operator<(const Complex& rhs) const {
                return amplitude() < rhs.amplitude();
            };

            /**
             * Operator overloading of ==
             * Checks for the equality of both real and imag part
             * @param rhs
             * @return result of this == rhs
             */
            bool operator==(const Complex& rhs) const {
                return complex_ == rhs.complex_;
            };

            friend inline std::ostream& operator<<(std::ostream& os, const Complex& obj) {
                os << " " << obj.real() << " + " << obj.imag() << "i ";
            }

            /**
             * Returns the real part of the complex
             * @return real
             */
            value_type real() const {
                return complex_.real();
            };

            void real(value_type real) {
                complex_.real(real);
            }

            /**
             * Returns the imag part of the complex
             * @return 
             */
            value_type imag() const {
                return complex_.imag();
            };

            void imag(value_type imag) {
                complex_.imag(imag);
            }

            /**
             * Evaluates and returns the amplitude of the complex
             * @return amplitude
             */
            value_type amplitude() const {
                return std::abs(complex_);
            };

            /**
             * Evaluates and returns the phase of the complex
             * @return phase in radians
             */
            value_type phase() const {
                return std::arg(complex_);
            };

            /**
             * Evaluates the intensity of the complex
             * @return intensity
             */
            value_type intensity() const {
                return std::norm(complex_);
            };

            /**
             * Changes the amplitude of the complex with the given value
             * @param amplitude
             */
            void set_amplitude(value_type amplitude) {
                value_type current_amplitude = this->amplitude();
                if (current_amplitude != 0.0) * this *= (amplitude / current_amplitude);

            };

            /**
             * Changes the phase of the complex with the given value
             * @param phase in radians
             */
            void set_phase(value_type phase) {
                double current_amplitude = this->amplitude();
                complex_ = std::complex<value_type>(current_amplitude * cos(phase), current_amplitude * sin(phase));
            };

            /**
             * Returns the conjugate of the current complex
             * @return Complex conjugate
             */
            Complex conjugate() {
                return std::conj(complex_);
            };


        private:

            std::complex<ValueType_> complex_;



        };
    }
}

#endif

