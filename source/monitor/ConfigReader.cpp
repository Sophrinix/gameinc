#include "stdafx.h"
#include "ConfigReader.h"
#include <IReadFile.h>


CConfigReader::CConfigReader()
{
    Text = "";
    TextPtr = Text.c_str();
}
//////////////////////////////////////////////////////////////////////

CConfigReader::CConfigReader( const stringw& filename )
{
    Load( filename );														//инициализируем данные
																				//из файла
}
//////////////////////////////////////////////////////////////////////

CConfigReader::CConfigReader( std::wistream& stream )
{
    Load( stream );														//из потока
}
//////////////////////////////////////////////////////////////////////

//! Loads a config file from a stream.
bool CConfigReader::Load( std::wistream& stream )
{
    wchar_t c;
    while ( stream.get(c).good() )
    {
        Text += c;
    }
    
    TextPtr = Text.c_str();
    
    return ( Text != L"" );
}
//////////////////////////////////////////////////////////////////////

//! Loads a config file
bool CConfigReader::Load( const stringw& filename ) 
{
    std::wifstream stream ( filename.c_str(), std::ios::in );
    
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
    
    const wchar_t* &c = TextPtr; // Called 'c' for convenience. It is a reference to TextPtr.
    
    do  // Do-while-loop takes care of syntax errors.
    {   // Incomplete assignments will cause the loop to start over, while correct syntax will end it after first iteration.
        
        // Find the beginning of an assignment.
        while ( *c == L' ' || *c == L'\n' || *c == L'\r' || *c == L'\t' || *c == L'#' )
        {
            // Skip whitespace
            while ( *c == L' ' || *c == L'\n' || *c == L'\r' || *c == L'\t' )
            {
                c++;
            }
            
            // Skip comments
            if ( *c == L'#' )
            {
                // Move to end of line
                while ( *c != 0 && *c != L'\n' && *c != L'\r' )
                    c++;
            }
        }
        
        // End of file reached?
        if ( *c == 0 )
            return false;
        
        // Pick up the name
        while ( *c != 0 && *c != L' ' && *c != L'\t' && *c != L'\n' && *c != L'\r' && *c != L'=' )
        {
            configName_ += *c;
            c++;
        }
        
        // Find the equal sign
        while ( *c != 0 && *c != L'=' && *c != L'\n' && *c != L'\r' )
        {
            c++;
        }
    
        // End of do-while-loop;
        // In case of an incomplete assignment, this will make sure we ignore it and keep moving rather than just return false.
    } while ( *c == 0 || *c == L'\n' || *c == L'\r' );
    
    // Go past the '=' sign
    c++;
    
    // Skip the whitespace between '=' and the value.
    while ( *c == L' ' || *c == L'\t' )
    {
        c++;
    }
    
    stringw tmp = L"";
    
    // Pick up value
    while ( *c != 0 && *c != L'\n' && *c != L'\r' && *c != L'#' )  // Stop at end of line, end of file, or beginning of a comment
    {
        if ( *c == L' ' || *c == L'\t' )
        {
            tmp = L"";
            
            // We don't want trailing spaces, so parse spaces until we know if they should be included or not
            while ( *c == L' ' || *c == L'\t' )
            {
                tmp += *c;
                c++;
            }
            
            // Did we NOT reach the end of line?
            if ( *c != 0 && *c != L'\n' && *c != L'\r' && *c != L'#' )
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
const stringw& CConfigReader::GetValue() const
{
    return configValue_;
}

//! Returns the name of the variable that has been assigned.
const stringw& CConfigReader::GetName() const
{
    return configName_;
}
