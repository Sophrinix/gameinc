#ifndef _C_CONFIG_READER_H_
#define _C_CONFIG_READER_H_

#include <fstream>

namespace irr
{
namespace io
{
    class IReadFile;
}
}

using namespace irr;

class CConfigReader
{
public:
    //! Creates a config reader. Call loadFile or loadStream to parse a config file with it.
    CConfigReader();
    
    //! Creates a config reader, reading from a file. The reader does not keep the file open, it buffers it all.
    CConfigReader( const stringw& filename );
    
    //! Creates a config reader, reading from a stream.
    //! The config reader no longer needs the stream when this constructor returns, so you can safely close it right after.
    CConfigReader( std::wistream& stream );
    
    //! Loads a config file from a stream. 
    //! The config reader no longer needs the stream when this method returns, so you can safely close it right after.
    bool Load( std::wistream& stream );
    
    //! Loads a config file
    bool Load( const stringw& filename );

    //! Loads a config file from an irrlicht read file.
    bool Load( io::IReadFile* file );
    
    //! Moves on to the next assignment. This should be called before getConfigValue() and getConfigName().
    bool Next();
    
    //! Returns the value of the assignment.
    const stringw& GetValue() const;
    
    //! Returns the name of the variable that has been assigned.
    const stringw& GetName() const;
    
private:
    stringw Text;
    const wchar_t* TextPtr;    // 'TextPtr' points to a char inside 'Text'.
    
    stringw configValue_;
    stringw configName_;
};

#endif
