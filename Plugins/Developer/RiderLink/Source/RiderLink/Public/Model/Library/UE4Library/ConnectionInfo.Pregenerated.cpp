//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a RdGen v1.13.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------
#include "ConnectionInfo.Pregenerated.h"



#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable:4250 )
#pragma warning( disable:4307 )
#pragma warning( disable:4267 )
#pragma warning( disable:4244 )
#pragma warning( disable:4100 )
#endif

namespace JetBrains {
namespace EditorPlugin {
// companion
// constants
// initializer
void ConnectionInfo::initialize()
{
}
// primary ctor
ConnectionInfo::ConnectionInfo(rd::Wrapper<std::wstring> projectName_, rd::Wrapper<std::wstring> executableName_, int32_t processId_) :
rd::IPolymorphicSerializable()
,projectName_(std::move(projectName_)), executableName_(std::move(executableName_)), processId_(std::move(processId_))
{
    initialize();
}
// secondary constructor
// default ctors and dtors
// reader
ConnectionInfo ConnectionInfo::read(rd::SerializationCtx& ctx, rd::Buffer & buffer)
{
    auto projectName_ = buffer.read_wstring();
    auto executableName_ = buffer.read_wstring();
    auto processId_ = buffer.read_integral<int32_t>();
    ConnectionInfo res{std::move(projectName_), std::move(executableName_), std::move(processId_)};
    return res;
}
// writer
void ConnectionInfo::write(rd::SerializationCtx& ctx, rd::Buffer& buffer) const
{
    buffer.write_wstring(projectName_);
    buffer.write_wstring(executableName_);
    buffer.write_integral(processId_);
}
// virtual init
// identify
// getters
std::wstring const & ConnectionInfo::get_projectName() const
{
    return *projectName_;
}
std::wstring const & ConnectionInfo::get_executableName() const
{
    return *executableName_;
}
int32_t const & ConnectionInfo::get_processId() const
{
    return processId_;
}
// intern
// equals trait
bool ConnectionInfo::equals(rd::ISerializable const& object) const
{
    auto const &other = dynamic_cast<ConnectionInfo const&>(object);
    if (this == &other) return true;
    if (this->projectName_ != other.projectName_) return false;
    if (this->executableName_ != other.executableName_) return false;
    if (this->processId_ != other.processId_) return false;
    
    return true;
}
// equality operators
bool operator==(const ConnectionInfo &lhs, const ConnectionInfo &rhs) {
    if (lhs.type_name() != rhs.type_name()) return false;
    return lhs.equals(rhs);
}
bool operator!=(const ConnectionInfo &lhs, const ConnectionInfo &rhs){
    return !(lhs == rhs);
}
// hash code trait
size_t ConnectionInfo::hashCode() const noexcept
{
    size_t __r = 0;
    __r = __r * 31 + (rd::hash<std::wstring>()(get_projectName()));
    __r = __r * 31 + (rd::hash<std::wstring>()(get_executableName()));
    __r = __r * 31 + (rd::hash<int32_t>()(get_processId()));
    return __r;
}
// type name trait
std::string ConnectionInfo::type_name() const
{
    return "ConnectionInfo";
}
// static type name trait
std::string ConnectionInfo::static_type_name()
{
    return "ConnectionInfo";
}
// polymorphic to string
std::string ConnectionInfo::toString() const
{
    std::string res = "ConnectionInfo\n";
    res += "\tprojectName = ";
    res += rd::to_string(projectName_);
    res += '\n';
    res += "\texecutableName = ";
    res += rd::to_string(executableName_);
    res += '\n';
    res += "\tprocessId = ";
    res += rd::to_string(processId_);
    res += '\n';
    return res;
}
// external to string
std::string to_string(const ConnectionInfo & value)
{
    return value.toString();
}
}
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

