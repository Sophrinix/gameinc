#ifndef _INCLUDE_GAMEMARKET_
#define _INCLUDE_GAMEMARKET_

#include "nrpConfig.h"
#include "nrpArrays.h"

namespace nrp
{

class NrpTime;

class CNrpGameMarket : public INrpConfig
{
public:
    static CNrpGameMarket& Instance();

    ~CNrpGameMarket(void);

    CNrpGame* GetGame( const NrpText& name );
    CNrpGame* GetGame( u32 index );
    const GAMES& GetGames() { return _games; }
    void AddGame( CNrpGame& ptrGame );

    void AddGameEngine( nrp::CNrpGameEngine* ptrEngine );
    CNrpGameEngine* GetGameEngine( const NrpText& name );
    void RemoveGameEngine( nrp::CNrpGameEngine* ptrEngine );

    bool AddPlatform( CNrpPlatform* platform );
    void RemovePlatform( const NrpText& name );
    CNrpPlatform* GetPlatform( const NrpText& name );
    CNrpPlatform* GetPlatform( size_t index );

    void StartSale( CNrpGame& game );

    CNrpRetailer* GetRetailer( const NrpText& name );
    void RemoveRetailer( const NrpText& name );

    float GetGameGenreInterest( CNrpGame& game );

    void BeginNewDay();
    void BeginNewMonth();

    void Load( const NrpText& folderName );
    NrpText Save( const NrpText& fileName );

    int GetPlatformSales( CNrpGame& game, NrpTime& endTime );

    static NrpText ClassName();
    static int CalcAmpChange( int curValue, int amp, int minPr );

    static const NrpText saveTemplate;
private:
    CNrpGameMarket(void);

    void _UpdateMarketGames();
    int _GetSalesNumber( CNrpGame& game );
    void _UpdateGameRating( CNrpGame& ptrGame );
   
    GAMES _games;
    PLATFORMS _platforms;
    RETAILERS _retailers;
    ENGINES _engines;
};

}
#endif