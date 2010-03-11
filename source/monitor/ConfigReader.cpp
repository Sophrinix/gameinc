#include "stdafx.h"
#include "ConfigReader.h"
#include <IReadFile.h>


CConfigReader::CConfigReader()
{
    Text = "";
    TextPtr = Text.c_str();
}
//////////////////////////////////////////////////////////////////////

CConfigReader::CConfigReader( const char* filename )
{
    Load( filename );														//инициализируем данные
																				//из файла
}
//////////////////////////////////////////////////////////////////////

CConfigReader::CConfigReader( std::istream& stream )
{
    Load( stream );														//из потока
}
//////////////////////////////////////////////////////////////////////

//! Loads a config file from a stream.
bool CConfigReader::Load( std::istream& stream )
{
    char c;
    while ( stream.get(c).good() )
    {
        Text += c;
    }
    
    TextPtr = Text.c_str();
    
    return ( Text != "" );
}
//////////////////////////////////////////////////////////////////////

//! Loads a config file
bool CConfigReader::Load( const char* filename ) 
{
    std::ifstream stream ( filename, std::ios::in );
    
    bool success = Load( stream );
    
    stream.close();
    
    return success;
}
//////////////////////////////////////////////////////////////////////

bool CConfigReader::Load( io::IReadFile* file )
{
    if ( !file ) 
        return false;

    int size = file->getSize();

    char* buffer = new char[size+1];

    file->read( buffer, size );

    Text = buffer;
    TextPtr = Text.c_str();

    delete [] buffer;

    return (Text != "");
}
//////////////////////////////////////////////////////////////////////

//! Moves on to the next assignment. This should be called before getConfigValue() and getConfigName().
bool CConfigReader::Next()
{
    // Reset value and name
    configValue_ = "";
    configName_ = "";
    
    const char* &c = TextPtr; // Called 'c' for convenience. It is a reference to TextPtr.
    
    do  // Do-while-loop takes care of syntax errors.
    {   // Incomplete assignments will cause the loop to start over, while correct syntax will end it after first iteration.
        
        // Find the beginning of an assignment.
        while ( *c == ' ' || *c == '\n' || *c == '\r' || *c == '\t' || *c == '#' )
        {
            // Skip whitespace
            while ( *c == ' ' || *c == '\n' || *c == '\r' || *c == '\t' )
            {
                c++;
            }
            
            // Skip comments
            if ( *c == '#' )
            {
                // Move to end of line
                while ( *c != 0 && *c != '\n' && *c != '\r' )
                    c++;
            }
        }
        
        // End of file reached?
        if ( *c == 0 )
            return false;
        
        // Pick up the name
        while ( *c != 0 && *c != ' ' && *c != '\t' && *c != '\n' && *c != '\r' && *c != '=' )
        {
            configName_ += *c;
            c++;
        }
        
        // Find the equal sign
        while ( *c != 0 && *c != '=' && *c != '\n' && *c != '\r' )
        {
            c++;
        }
    
        // End of do-while-loop;
        // In case of an incomplete assignment, this will make sure we ignore it and keep moving rather than just return false.
    } while ( *c == 0 || *c == '\n' || *c == '\r' );
    
    // Go past the '=' sign
    c++;
    
    // Skip the whitespace between '=' and the value.
    while ( *c == ' ' || *c == '\t' )
    {
        c++;
    }
    
    std::string tmp = "";
    
    // Pick up value
    while ( *c != 0 && *c != '\n' && *c != '\r' && *c != '#' )  // Stop at end of line, end of file, or beginning of a comment
    {
        if ( *c == ' ' || *c == '\t' )
        {
            tmp = "";
            
            // We don't want trailing spaces, so parse spaces until we know if they should be included or not
            while ( *c == ' ' || *c == '\t' )
            {
                tmp += *c;
                c++;
            }
            
            // Did we NOT reach the end of line?
            if ( *c != 0 && *c != '\n' && *c != '\r' && *c != '#' )
                configValue_ += tmp; // There is more text, so we include all the whitespace
            
            // Continue parsing, or break if end of line was reached
            continue;
        }
        
        configValue_ += *c;
        c++;
    }
    
    return true;
}

//! Returns the value of the assignment.
const char* CConfigReader::GetValue() const
{
    return configValue_.c_str();
}

//! Returns the name of the variable that has been assigned.
const char* CConfigReader::GetName() const
{
    return configName_.c_str();
}
