//
// Generated file, do not edit! Created by nedtool 5.4 from accept.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __ACCEPT_M_H
#define __ACCEPT_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0504
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>accept.msg:16</tt> by nedtool.
 * <pre>
 * //
 * // This program is free software: you can redistribute it and/or modify
 * // it under the terms of the GNU Lesser General Public License as published by
 * // the Free Software Foundation, either version 3 of the License, or
 * // (at your option) any later version.
 * // 
 * // This program is distributed in the hope that it will be useful,
 * // but WITHOUT ANY WARRANTY; without even the implied warranty of
 * // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * // GNU Lesser General Public License for more details.
 * // 
 * // You should have received a copy of the GNU Lesser General Public License
 * // along with this program.  If not, see http://www.gnu.org/licenses/.
 * //
 * message Accept
 * {
 *     int source;
 *     int color;
 * }
 * </pre>
 */
class Accept : public ::omnetpp::cMessage
{
  protected:
    int source;
    int color;

  private:
    void copy(const Accept& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Accept&);

  public:
    Accept(const char *name=nullptr, short kind=0);
    Accept(const Accept& other);
    virtual ~Accept();
    Accept& operator=(const Accept& other);
    virtual Accept *dup() const override {return new Accept(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getSource() const;
    virtual void setSource(int source);
    virtual int getColor() const;
    virtual void setColor(int color);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const Accept& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, Accept& obj) {obj.parsimUnpack(b);}


#endif // ifndef __ACCEPT_M_H

