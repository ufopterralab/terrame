/************************************************************************************
TerraLib - a library for developing GIS applications.
Copyright � 2001-2007 INPE and Tecgraf/PUC-Rio.

This code is part of the TerraLib library.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

You should have received a copy of the GNU Lesser General Public
License along with this library.

The authors reassure the license terms regarding the warranties.
They specifically disclaim any warranties, including, but not limited to,
the implied warranties of merchantability and fitness for a particular purpose.
The library provided hereunder is on an "as is" basis, and the authors have no
obligation to provide maintenance, support, updates, enhancements, or modifications.
In no event shall INPE and Tecgraf / PUC-Rio be held liable to any party for direct,
indirect, special, incidental, or consequential damages arising out of the use
of this library and its documentation.
*************************************************************************************/
/*! \file luaCell.cpp
    \brief This file contains the implementation for the luaCell objects.
        \author Tiago Garcia de Senna Carneiro
*/

#include "luaCell.h"
#include "luaNeighborhood.h"

// Observadores
#include "../observer/types/observerTextScreen.h"
#include "../observer/types/observerGraphic.h"
#include "../observer/types/observerLogFile.h"
#include "../observer/types/observerTable.h"
#include "../observer/types/observerUDPSender.h"
// #include "../observer/types/observerScheduler.h"
#include "../observer/types/agentObserverMap.h"
#include "luaUtils.h"

#define TME_STATISTIC_UNDEF

#ifdef TME_STATISTIC 
    // Estatisticas de desempenho
    #include "../observer/statistic/statistic.h"
#endif

//@RODRIGO
// #include "../observer/protocol/session/serverSession.h"

///< Gobal variabel: Lua stack used for comunication with C++ modules.
extern lua_State * L; 

///< true - TerrME runs in verbose mode and warning messages to the user; 
/// false - it runs in quite node and no messages are shown to the user.
extern bool QUIET_MODE; 

/// Constructor
luaCell::luaCell(lua_State *L)
{  
    NeighCmpstInterf& nhgs = Cell::getNeighborhoods( );
    it = nhgs.begin();

    // Antonio
    luaL = L;
    subjectType = TObsCell;
    observedAttribs.clear();

    //@RODRIGO
    // serverSession = new ServerSession();
}

/// Returns the current internal state of the LocalAgent (Automaton) within the cell and received as parameter 
int luaCell::getCurrentStateName( lua_State *L )		
{
    luaLocalAgent *agent = Luna<luaLocalAgent>::check(L, -1);
    ControlMode* controlMode = getControlMode((LocalAgent*)agent);

    if( controlMode) lua_pushstring( L, controlMode->getControlModeName( ).c_str() );
    else lua_pushnil(L);

    return 1;
}

/// Puts the iterator in the beginning of the luaNeighborhood composite.
int luaCell::first(lua_State *){
    NeighCmpstInterf& nhgs = Cell::getNeighborhoods( );
    it = nhgs.begin();
    return 0;
}

/// Puts the iterator in the end of the luaNeighborhood composite.
int luaCell::last(lua_State *) {
    NeighCmpstInterf& nhgs = Cell::getNeighborhoods( );
    it = nhgs.end();
    return 1;
}

/// Returns true if the Neighborhood iterator is in the beginning of the Neighbor composite data structure  
/// no parameters
int luaCell::isFirst(lua_State *L) {
    NeighCmpstInterf& nhgs = Cell::getNeighborhoods( );
    lua_pushboolean(L, it == nhgs.begin());
    return  1;
}

/// Returns true if the Neighborhood iterator is in the end of the Neighbor composite data structure  
/// no parameters
int luaCell::isLast(lua_State *L) {
    NeighCmpstInterf& nhgs = Cell::getNeighborhoods( );
    lua_pushboolean(L, it == nhgs.end());
    return  1;
}

/// Returns true if the Neighborhood is empty.
/// no parameters
int luaCell::isEmpty(lua_State *L) {  
    NeighCmpstInterf& nhgs = Cell::getNeighborhoods( );
    lua_pushboolean(L, nhgs.empty() );
    return 1;
}

/// Clears all the Neighborhood content
/// no parameters
int luaCell::clear(lua_State *) {  
    NeighCmpstInterf& nhgs = Cell::getNeighborhoods( );
    nhgs.clear( );
    return 0;
}

/// Returns the number of Neighbors cells in the Neighborhood
int luaCell::size(lua_State *) {  
    NeighCmpstInterf& nhgs = Cell::getNeighborhoods( );
    lua_pushnumber(L,nhgs.size( ));
    return 1;
}

/// Fowards the Neighborhood iterator to the next Neighbor cell
// no parameters
int luaCell::next( lua_State * )
{
    NeighCmpstInterf& nhgs = Cell::getNeighborhoods( );
    if( it != nhgs.end() ) it++;
    return 0;
}

/// destructor
luaCell::~luaCell( void ) { luaL_unref( L, LUA_REGISTRYINDEX, ref); }

/// Sets the Cell latency
int luaCell::setLatency(lua_State *L) { Cell::setLatency(luaL_checknumber(L, 1)); return 0; }

/// Gets the Cell latency
int luaCell::getLatency(lua_State *L) { lua_pushnumber(L, Cell::getLatency()); return 1; }

/// Sets the neighborhood
int luaCell::setNeighborhood(lua_State *) { 
    //	luaNeighborhood* neigh = Luna<luaNeighborhood>::check(L, -1);
    return 0;
}

/// Gets the current active luaNeighboorhood
int luaCell::getCurrentNeighborhood(lua_State *L) { 

    NeighCmpstInterf& nhgs = Cell::getNeighborhoods( );
    if( it !=  nhgs.end() )
    {
        luaNeighborhood* neigh = (luaNeighborhood*) it->second;

        if( neigh != NULL )
            neigh->getReference(L);
        else
            lua_pushnil( L );

    }
    else
        lua_pushnil( L );

    return 1;
}

/// Returns the Neihborhood graph which name has been received as a parameter
int luaCell::getNeighborhood(lua_State *L) { 

    NeighCmpstInterf& neighs = Cell::getNeighborhoods();

    // Get and test parameters
    const char* charIndex = luaL_checkstring(L, -1);
    string index = string( charIndex );
    if( neighs.empty() ) lua_pushnil(L); // return nil
    else
    {
        // Get the cell	neighborhood
        NeighCmpstInterf::iterator location = neighs.find( index );
        if ( location == neighs.end())
        {
            string err_out = string("Erro: neighborhood \"" ) + string (index) + string("\" not found.\n");
            qFatal( "%s", err_out.c_str() );

            lua_pushnil( L );
            return 1;

        }
        luaNeighborhood* neigh = (luaNeighborhood*) location->second;

        if( neigh != NULL )
            neigh->getReference(L);
        else
            lua_pushnil( L );
    }

    return 1;
}

/// Adds a new luaNeighborhood graph to the Cell
/// parameters: identifier, luaNeighborhood 
int luaCell::addNeighborhood( lua_State *L )
{
    string id = string( luaL_checkstring(L, -2) );
    luaNeighborhood* neigh = Luna<luaNeighborhood>::check(L, -1);
    NeighCmpstInterf& neighs = Cell::getNeighborhoods();
    pair< string, CellNeighborhood*> pStrNeigh;
    neigh->CellNeighborhood::setID(id);
    pStrNeigh.first = id;
    pStrNeigh.second = neigh;
    neighs.erase(id );
    neighs.add( pStrNeigh );
    it = neighs.begin();
    return 0;
}

/// Synchronizes the luaCell
int luaCell::synchronize(lua_State *) { 
    Cell::synchronize( sizeof(luaCell) ); // parametro n?o testado
    return 0;
}

/// Registers the luaCell object in the Lua stack
int luaCell::setReference( lua_State* L)
{
    ref = luaL_ref(L, LUA_REGISTRYINDEX );
    return 0;
}

/// Gets the luaCell object reference
int luaCell::getReference( lua_State *L )
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
    return 1;
}

/// Gets the luaCell identifier 
int luaCell::getID( lua_State *L )
{
    lua_pushstring(L, objectId_.c_str() );
    return 1;
}

//@RAIAN
/// Gets the luaCell identifier
/// \author Raian Vargas Maretto
const char* luaCell::getID( )
{
	return this->objectId_.c_str();
}
//@RAIAN: Fim.

/// Sets the luaCell identifier
int luaCell::setID( lua_State *L )
{
    const char* id = luaL_checkstring( L , -1);
    objectId_ = string( id );
    return 0;
}

//Raian
/// Sets the cell index (x,y)
/// Parameters: cell.x, cell.y
/// \author Raian Vargas Maretto
int luaCell::setIndex(lua_State *L)
{
	this->idx.second = luaL_checknumber(L, -1);
	this->idx.first = luaL_checknumber(L, -2);
	return 0;
}

//Raian
/// Gets the cell index (x,y)
/// \author Raian Vargas Maretto
CellIndex luaCell::getIndex()
{
	return this->idx;
}

/// Creates several types of observers
/// parameters: observer type, observeb attributes table, observer type parameters
// verif. ref (endereco na pilha lua)
// olhar a classe event
int luaCell::createObserver( lua_State * )
{	
    // recupero a referencia da celula
    lua_rawgeti(luaL, LUA_REGISTRYINDEX, ref);
            
    // flags para a defini��o do uso de compress�o
    // na transmiss�o de datagramas e da visibilidade
    // dos observadores Udp Sender 
    bool compressDatagram = false, obsVisible = true;

    // recupero a tabela de
    // atributos da celula
    int top = lua_gettop(luaL);

    // Nao modifica em nada a pilha
    // recupera o enum referente ao tipo
    // do observer
    int typeObserver = (int)luaL_checkinteger(luaL, -4);
	
	//@RAIAN
	// Para o Observer do tipo Neighbohrood
    if( typeObserver != TObsNeigh )
	{  
		bool isGraphicType = (typeObserver == TObsDynamicGraphic) || (typeObserver == TObsGraphic);

		//------------------------
		QStringList allAttribs, obsAttribs;

		// Pecorre a pilha lua recuperando todos os atributos celula
		lua_pushnil(luaL);
		while(lua_next(luaL, top) != 0)
		{
		    QString key( luaL_checkstring(luaL, -2) );

		    allAttribs.push_back(key);
		    lua_pop(luaL, 1);
		}

		//------------------------
		// pecorre a pilha lua recuperando
		// os atributos celula que se quer observar
		lua_settop(luaL, top - 1);
		top = lua_gettop(luaL);

		// Verificacao da sintaxe da tabela Atributos
		if(! lua_istable(luaL, top) )
		{
		    qFatal("Error: Attributes table not found. Incorrect sintax.\n");
		    return -1;
		}

		bool attribTable = false;

		lua_pushnil(luaL);
		while(lua_next(luaL, top - 1 ) != 0)
		{
		    QString key( luaL_checkstring(luaL, -1) );
		    attribTable = true;

		    // Verifica se o atributo informado n�o existe deve ter sido digitado errado
		    if (allAttribs.contains(key))
		    {
		        obsAttribs.push_back(key);
		        if (! observedAttribs.contains(key))
		            observedAttribs.push_back(key);
		    }
		    else
		    {
		        if ( ! key.isNull() || ! key.isEmpty())
		        {
		            qFatal("Error: Attribute name '%s' not found.\n", qPrintable(key));
		            return -1;
		        }
		    }
		    lua_pop(luaL, 1);
		}
		//------------------------

		// if ((obsAttribs.empty() ) && (! isGraphicType))
		if (obsAttribs.empty())
		{
		    obsAttribs = allAttribs;
		    observedAttribs = allAttribs;
		}

		//if(! lua_istable(luaL, top) )
		//{
		//    qWarning("Warning: Parameter table not found. Incorrect sintax.");
		//    return 0;
		//}

		QStringList cols, obsParams;

		// Recupera a tabela de parametros os observadores do tipo Table e Graphic
		// caso n�o seja um tabela a sintaxe do metodo esta incorreta
		lua_pushnil(luaL);
		while(lua_next(luaL, top) != 0)
		{   
		    QString key;
		    if (lua_type(luaL, -2) == LUA_TSTRING)
		        key = QString(luaL_checkstring(luaL, -2));

		    switch (lua_type(luaL, -1))
		    {
		    case LUA_TSTRING:
		        {
		            QString value( luaL_checkstring(luaL, -1));
		            cols.push_back(value);
		            break;
		        }

		    case LUA_TBOOLEAN:
		        {
		            bool val = lua_toboolean(luaL, -1);
		            if (key == "visible")
		                obsVisible = val;
		            else // if (key == "compress")
		                compressDatagram = val;
                    break;
		        }

		    case LUA_TTABLE:
		        {
                    int tableTop = lua_gettop(luaL);
                                           
		            lua_pushnil(luaL);
		            while(lua_next(luaL, tableTop) != 0)
                    {
		                if (lua_type(luaL, -2) == LUA_TSTRING)
		                    obsParams.append(luaL_checkstring(luaL, -2));

		                switch (lua_type(luaL, -1))
                        {
                            case LUA_TNUMBER:
                                cols.append(QString::number(luaL_checknumber(luaL, -1)) );
                                break;
                            
                            case LUA_TSTRING:
		                        cols.append(luaL_checkstring(luaL, -1));
                                break;
                        }
            		    lua_pop(luaL, 1);
                    }
                }

		    default:
		        break;
		    }
		    lua_pop(luaL, 1);
		}

		// Caso n�o seja definido nenhum parametro,
		// e o observador n�o � TextScreen ent�o
		// lan�a um warning
		if ((cols.isEmpty()) && (typeObserver != TObsTextScreen))
		{
		    if (! QUIET_MODE )
		        qWarning("Warning: The Parameters Table is empty.");
		}
		//------------------------

		ObserverTextScreen *obsText = 0;
		ObserverTable *obsTable = 0;
		ObserverGraphic *obsGraphic = 0;
		ObserverLogFile *obsLog = 0;
		ObserverUDPSender *obsUDPSender = 0;

		int obsId = -1;

		switch (typeObserver)
		{
		    case TObsTextScreen:
		        obsText = (ObserverTextScreen*) 
		            CellSubjectInterf::createObserver(TObsTextScreen);
		        if (obsText)
		        {
		            obsId = obsText->getId();
		        }
		        else
		        {
		            if (! QUIET_MODE)
		                qWarning("%s", qPrintable(TerraMEObserver::MEMORY_ALLOC_FAILED));
		        }
		        break;

		    case TObsLogFile:
		        obsLog = (ObserverLogFile*) 
		            CellSubjectInterf::createObserver(TObsLogFile);
		        if (obsLog)
		        {
		            obsId = obsLog->getId();
		        }
		        else
		        {
		            if (! QUIET_MODE)
		                qWarning("%s", qPrintable(TerraMEObserver::MEMORY_ALLOC_FAILED));
		        }
		        break;

		    case TObsTable:
		        obsTable = (ObserverTable *) 
		            CellSubjectInterf::createObserver(TObsTable);
		        if (obsTable)
		        {
		            obsId = obsTable->getId();
		        }
		        else
		        {
		            if (! QUIET_MODE)
		                qWarning("%s", qPrintable(TerraMEObserver::MEMORY_ALLOC_FAILED));
		        }
		        break;

		    case TObsDynamicGraphic:
		        obsGraphic = (ObserverGraphic *) 
		            CellSubjectInterf::createObserver(TObsDynamicGraphic);
		        if (obsGraphic)
		        {
		            obsGraphic->setObserverType(TObsDynamicGraphic);
		            obsId = obsGraphic->getId();
		        }
		        else
		        {
		            if (! QUIET_MODE)
		                qWarning("%s", qPrintable(TerraMEObserver::MEMORY_ALLOC_FAILED));
		        }
		        break;

		    case TObsGraphic:
		        obsGraphic = (ObserverGraphic *) 
		            CellSubjectInterf::createObserver(TObsGraphic);
		        if (obsGraphic)
		        {
		            obsId = obsGraphic->getId();
		        }
		        else
		        {
		            if (! QUIET_MODE)
		                qWarning("%s", qPrintable(TerraMEObserver::MEMORY_ALLOC_FAILED));
		        }
		        break;

		    case TObsUDPSender:
		        obsUDPSender = (ObserverUDPSender *) 
		            CellSubjectInterf::createObserver(TObsUDPSender);
		        if (obsUDPSender)
		        {
		            obsId = obsUDPSender->getId();
		            obsUDPSender->setCompressDatagram(compressDatagram);

		            if (obsVisible)
		                obsUDPSender->show();
		        }
		        else
		        {
		            if (! QUIET_MODE)
		                qWarning("%s", qPrintable(TerraMEObserver::MEMORY_ALLOC_FAILED));
		        }
		        break;

		    case TObsMap:
		    default:
		        if (! QUIET_MODE )
		        {
		            qWarning("Warning: In this context, the code '%s' does not correspond to a "
		                     "valid type of Observer.",  getObserverName(typeObserver) );
		        }
		        return 0;
		}

#ifdef DEBUG_OBSERVER
        qDebug() << "luaCell";
        qDebug() << "obsParams: " << obsParams;
        qDebug() << "obsAttribs: " << obsAttribs;
        qDebug() << "allAttribs: " << allAttribs;
        qDebug() << "cols: " << cols;
#endif

		//@RODRIGO
		//serverSession->add(obsKey);

		/// Define alguns parametros do observador instanciado ---------------------------------------------------

		if (obsLog)
		{
		    obsLog->setAttributes(obsAttribs);

		    if (cols.at(0).isNull() || cols.at(0).isEmpty())
		    {
		        if (! QUIET_MODE )
		            qWarning("Warning: Filename was not specified, using a "
		                     "default \"%s\".", qPrintable(DEFAULT_NAME));
		        obsLog->setFileName(DEFAULT_NAME + ".csv");
		    }
		    else
		    {
		        obsLog->setFileName(cols.at(0));
		    }

		    // caso n�o seja definido, utiliza o default ";"
		    if ((cols.size() < 2) || cols.at(1).isNull() || cols.at(1).isEmpty())
		    {
		        if (! QUIET_MODE )
		            qWarning("Warning: Separator not defined, using \";\".");
		        obsLog->setSeparator();
		    }
		    else
		    {
		        obsLog->setSeparator(cols.at(1));
		    }

		    lua_pushnumber(luaL, obsId);
		    return 1;
		}

		if (obsText)
		{
		    obsText->setAttributes(obsAttribs);
		    lua_pushnumber(luaL, obsId);
		    return 1;
		}

		if (obsTable)
		{
		    if ((cols.size() < 2) || cols.at(0).isNull() || cols.at(0).isEmpty()
		            || cols.at(1).isNull() || cols.at(1).isEmpty())
		    {
		        if (! QUIET_MODE )
		            qWarning("Warning: Column title not defined.");
		    }

		    obsTable->setColumnHeaders(cols);
		    obsTable->setAttributes(obsAttribs);

		    lua_pushnumber(luaL, obsId);
		    return 1;
		}

		if (obsGraphic)
		{
            obsGraphic->setLegendPosition();

            // Takes titles of three first locations
            obsGraphic->setTitles(cols.at(0), cols.at(1), cols.at(2));   
            cols.removeFirst(); // remove graphic title
            cols.removeFirst(); // remove axis x title
            cols.removeFirst(); // remove axis y title

            // Splits the attribute labels in the cols list
            obsGraphic->setAttributes(obsAttribs, cols.takeFirst().split(";", QString::SkipEmptyParts),
                obsParams, cols);

		    lua_pushnumber(luaL, obsId);
		    return 1;
		}

		if(obsUDPSender)
		{
		    obsUDPSender->setAttributes(obsAttribs);

		    // if (cols.at(0).isEmpty())
		    if (cols.isEmpty())
		    {
		        if (! QUIET_MODE )
		            qWarning("Warning: Port not defined.");
		    }
		    else
		    {
		        obsUDPSender->setPort(cols.at(0).toInt());
		    }

		    // broadcast
		    if ((cols.size() == 1) || ((cols.size() == 2) && cols.at(1).isEmpty()) )
		    {
		        if (! QUIET_MODE )
		            qWarning("Warning: Observer will send to broadcast.");
		        obsUDPSender->addHost(BROADCAST_HOST);
		    }
		    else
		    {
		        // multicast or unicast
		        for(int i = 1; i < cols.size(); i++){
		            if (! cols.at(i).isEmpty())
		                obsUDPSender->addHost(cols.at(i));
		        }
		    }
		    lua_pushnumber(luaL, obsId);
		    return 1;
		}
    }
	//@RAIAN
	// Comeca a criacao do Observer do tipo Neighborhood
	else
	{
		QStringList obsParams, obsParamsAtribs;

		bool getObserverID = false, isLegend = false;
		int obsID = -1;

		AgentObserverMap *obsMap = 0;
		luaCellularSpace *cellSpace = 0;

		// Recupera os parametros
		lua_pushnil(luaL);
		while(lua_next(luaL, top - 1) != 0)
		{
			// Recupera o ID do observer map
			if( lua_isnumber(luaL, -1) && (!getObserverID) )
			{
				obsID = luaL_checknumber(luaL, -1);
				getObserverID = true;
				isLegend = true;
			}

			//Recupera o espaco celular e a legenda
			if(lua_istable(luaL, -1))
			{
				int paramTop = lua_gettop(luaL);

				lua_pushnil(luaL);
				while(lua_next(luaL, paramTop) != 0)
				{
					if(isudatatype(luaL, -1, "TeCellularSpace"))
					{
						cellSpace = Luna<luaCellularSpace>::check(luaL, -1);
					}
					else
					{
						if(isLegend)
						{
							QString key(luaL_checkstring(luaL, -2));
							obsParams.push_back(key);

							bool boolAux;
							double numAux;
							QString strAux;

							switch(lua_type(luaL, -1))
							{
							case LUA_TBOOLEAN:
								boolAux = lua_toboolean(luaL, -1);
								break;

							case LUA_TNUMBER:
								numAux = luaL_checknumber(luaL, -1);
								obsParamsAtribs.push_back(QString::number(numAux));
								break;

							case LUA_TSTRING:
								strAux = luaL_checkstring(luaL, -1);
								obsParamsAtribs.push_back(QString(strAux));

							case LUA_TNIL:
							case LUA_TTABLE:
							default:
								;
							}
						}
					}
					lua_pop(luaL, 1);
				}
			}
			lua_pop(luaL, 1);
		}

		QString errorMsg = QString("\nError: The Observer ID \"%1\" was not found. "
			"Check the declaration of this observer.\n").arg(obsID);

		if(!cellSpace)
			qFatal("%s", qPrintable(errorMsg));

		QStringList neighIDs;
		QString exhibitionName;

		// Recupera os IDs das Vizinhancas a serem observadas
		lua_pushnil(luaL);
		while(lua_next(luaL, top - 2) != 0)
		{
			const char* key = luaL_checkstring(luaL, -1);
			exhibitionName.append(key);
			exhibitionName.push_front(" (");
			exhibitionName.push_back(")");

			neighIDs.push_back(QString("neighborhood") + exhibitionName);
			observedAttribs.push_back(QString("neighborhood") + exhibitionName);
#ifdef TME_BLACK_BOARD
			// Solucao provisoria para o observer do tipo Neighbohrood
			observedAttribs.push_front("@getNeighborhoodState");
#endif
			lua_pop(luaL, 1);
		}

        if(typeObserver == TObsNeigh)
		{
			obsMap = (AgentObserverMap *)cellSpace->getObserver(obsID);
			if(!obsMap)
				qFatal("%s", qPrintable(errorMsg));
			obsMap->registry(this, QString("neighborhood") + exhibitionName);
		}

        if(typeObserver == TObsNeigh)
		{
            obsMap->setAttributes(neighIDs, obsParams, obsParamsAtribs);
			obsMap->setSubjectAttributes(neighIDs, TObsNeighborhood, exhibitionName);
		}
	}
	//@RAIAN - FIM
    return 0;
}

const TypesOfSubjects luaCell::getType()
{
    return subjectType;
}

/// Notifies observers about changes in the luaCell internal state
int luaCell::notify(lua_State *L )
{
#ifdef DEBUG_OBSERVER
    printf("\ncell::notifyObservers\n");
    luaStackToQString(12);
# endif

    double time = luaL_checknumber(L, -1);

#ifdef TME_STATISTIC
    double t = Statistic::getInstance().startTime();

    CellSubjectInterf::notifyObservers(time);

    t = Statistic::getInstance().endTime() - t;
    Statistic::getInstance().addElapsedTime("Total Response Time - cell", t);
    Statistic::getInstance().collectMemoryUsage();
#else
    CellSubjectInterf::notify(time);
#endif
    return 0;
}

//@RODRIGO
QString luaCell::getAll(QDataStream & /*in*/, int /*observerId*/, QStringList& attribs)
{
    lua_rawgeti(luaL, LUA_REGISTRYINDEX, ref);	// recupero a referencia na pilha lua
    return pop(luaL, attribs);
}

QString luaCell::pop(lua_State *luaL, QStringList& attribs)
{
    double num = 0;
    bool boolAux = false;

    QString msg, attrs, key, text;
	
	int attrCounter = 0;
    int cellsPos = lua_gettop(luaL);
    // int type = lua_type (luaL, cellsPos);
	
	// @RAIAN: Serializa a vizinhanca
	if(attribs.contains("@getNeighborhoodState"))
	{
#ifndef TME_BLACK_BOARD
		// solucao provisoria
		attribs.pop_back();
#endif

		QString elements;

		NeighCmpstInterf neighborhoods = this->getNeighborhoods();
		NeighCmpstInterf::iterator itAux = neighborhoods.begin();

		while(itAux != neighborhoods.end())
		{
			QString neighborhoodID(itAux->first.c_str());

			if(attribs.contains(QString("neighborhood (") + neighborhoodID + QString(")")))
			{
				// Neighborhood ID
				msg.append(QString("neighborhood (") + neighborhoodID + QString(")"));
				msg.append(PROTOCOL_SEPARATOR);

				// subject TYPE
                msg.append(QString::number(TObsNeighborhood));
				msg.append(PROTOCOL_SEPARATOR);

				// Pega as informa��es da c�lula central (this)
				QString cellMsg = this->pop(luaL, QStringList() << "x" << "y");

				elements.append(cellMsg);

				CellNeighborhood *neigh = itAux->second;
				CellNeighborhood::iterator itNeigh = neigh->begin();
				int neighSize = neigh->size();

				//Number of Attributes - VARIFICAR SE ESTA CERTO
				msg.append(QString::number(0));
				msg.append(PROTOCOL_SEPARATOR);

				// Number of internal subjects
				msg.append(QString::number(neighSize + 1));
				msg.append(PROTOCOL_SEPARATOR);
				msg.append(PROTOCOL_SEPARATOR);

				while(itNeigh != neigh->end())
				{
					luaCell* neighbor = (luaCell*)itNeigh->second;
					CellIndex neighIdx = (CellIndex)itNeigh->first;
					double weight = neigh->getWeight(neighIdx);

					int ref = neighbor->getReference(luaL);
					cellMsg = neighbor->pop(luaL, QStringList() << "x" << "y" << "@getWeight");
					lua_settop(luaL, 0);

                    cellMsg.append(QString::number(TObsNumber));
					cellMsg.append(PROTOCOL_SEPARATOR);
					cellMsg.append(QString::number(weight));
					cellMsg.append(PROTOCOL_SEPARATOR);
					cellMsg.append(PROTOCOL_SEPARATOR);

					elements.append(cellMsg);

					itNeigh++;
				}
			}
			itAux++;
		}

		msg.append(elements);
		msg.append(PROTOCOL_SEPARATOR);
	}// @RAIAN: FIM
	else
	{

		// id
		msg.append("cell");
		msg.append(QString::number(this->ref)); // QString("%1").arg(this->ref));
		msg.append(PROTOCOL_SEPARATOR);

		// subjectType
		msg.append(QString::number(subjectType));
		msg.append(PROTOCOL_SEPARATOR);


		lua_pushnil(luaL);
		while(lua_next(luaL, cellsPos ) != 0)
		{
		    key = QString(luaL_checkstring(luaL, -2));

		    if( attribs.contains(key) )
		    {
		        attrCounter++;
		        attrs.append(key);
		        attrs.append(PROTOCOL_SEPARATOR);

		        switch( lua_type(luaL, -1) )
		        {
		            case LUA_TBOOLEAN:
		                boolAux = lua_toboolean(luaL, -1);
		                attrs.append(QString::number(TObsBool));
		                attrs.append(PROTOCOL_SEPARATOR);
		                attrs.append(QString::number(boolAux));
		                attrs.append(PROTOCOL_SEPARATOR);
		                break;

		            case LUA_TNUMBER:
		                num = luaL_checknumber(luaL, -1);
		                attrs.append(QString::number(TObsNumber));
		                attrs.append(PROTOCOL_SEPARATOR);
		                attrs.append(QString::number(num));
		                attrs.append(PROTOCOL_SEPARATOR);
		                break;

		            case LUA_TSTRING:
		                text = QString(luaL_checkstring(luaL, -1));
		                attrs.append(QString::number(TObsText));
		                attrs.append(PROTOCOL_SEPARATOR);
		                attrs.append( (text.isEmpty() || text.isNull() ? VALUE_NOT_INFORMED : text) );
		                attrs.append(PROTOCOL_SEPARATOR);
		                break;

		            case LUA_TTABLE:
		            {
		                char result[100];
		                sprintf( result, "%p", lua_topointer(luaL, -1) );
		                attrs.append(QString::number(TObsText));
		                attrs.append(PROTOCOL_SEPARATOR);
		                attrs.append(QString("Lua-Address(TB): ") + QString(result));
		                attrs.append(PROTOCOL_SEPARATOR);
		                break;
		            }
		            case LUA_TUSERDATA:
		            {
		                char result[100];
		                sprintf( result, "%p", lua_topointer(luaL, -1) );
		                attrs.append(QString::number(TObsText));
		                attrs.append(PROTOCOL_SEPARATOR);
		                attrs.append(QString("Lua-Address(UD): ") + QString(result));
		                attrs.append(PROTOCOL_SEPARATOR);
		                break;
		            }

		            case LUA_TFUNCTION:
		            {
		                char result[100];
		                sprintf(result, "%p", lua_topointer(luaL, -1) );
		                attrs.append(QString::number(TObsText) );
		                attrs.append(PROTOCOL_SEPARATOR);
		                attrs.append(QString("Lua-Address(FT): ") + QString(result));
		                attrs.append(PROTOCOL_SEPARATOR);
		                break;
		            }

		            default:
		            {
		                char result[100];
		                sprintf(result, "%p", lua_topointer(luaL, -1) );
		                attrs.append(QString::number(TObsText) );
		                attrs.append(PROTOCOL_SEPARATOR);
		                attrs.append(QString("Lua-Address(O): ") + QString(result));
		                attrs.append(PROTOCOL_SEPARATOR);
		                break;
		            }
		        }
		    }
		    lua_pop(luaL, 1);
		}
	
		//@RAIAN: Para uso na serializacao da Vizinhanca
		if(attribs.contains("@getWeight"))
		{
			attrCounter++;
			attrs.append("@getWeight");
			attrs.append(PROTOCOL_SEPARATOR);
		}
		//@RAIAN: FIM

		// #attrs
		msg.append(QString::number(attrCounter));
		msg.append(PROTOCOL_SEPARATOR );

		// #elements
		msg.append(QString::number(0));
		msg.append(PROTOCOL_SEPARATOR );

		msg.append(attrs);
		
		//@RAIAN: Para uso na serializacao da Vizinhanca
		if(!attribs.contains("@getWeight"))
			msg.append(PROTOCOL_SEPARATOR);
		//@RAIAN: FIM
	}

    return msg;
}

QString luaCell::getChanges(QDataStream& in, int observerId, QStringList& attribs)
{
    return getAll(in, observerId, attribs);
}

#ifdef TME_BLACK_BOARD
QDataStream& luaCell::getState(QDataStream& in, Subject *, int observerId, QStringList & /* attribs */)
#else
QDataStream& luaCell::getState(QDataStream& in, Subject *, int observerId, QStringList &  attribs )
#endif

{

#ifdef DEBUG_OBSERVER
    printf("\ngetState\n\nobsAttribs.size(): %i\n", obsAttribs.size());
    luaStackToQString(12);
#endif

    int obsCurrentState = 0; //serverSession->getState(observerId);
    QString content;

    switch(obsCurrentState)
    {
        case 0:
#ifdef TME_BLACK_BOARD
        content = getAll(in, observerId, observedAttribs);
#else
        content = getAll(in, observerId, attribs);
#endif
            // serverSession->setState(observerId, 1);
            // if (! QUIET_MODE )
            // qWarning(QString("Observer %1 passou ao estado %2").arg(observerId).arg(1).toAscii().constData());
            break;

        case 1:
#ifdef TME_BLACK_BOARD
        content = getChanges(in, observerId, observedAttribs);
#else
        content = getChanges(in, observerId, attribs);
#endif
            // serverSession->setState(observerId, 0);
            // if (! QUIET_MODE )
            // qWarning(QString("Observer %1 passou ao estado %2").arg(observerId).arg(0).toAscii().constData());
            break;
    }
    // cleans the stack
    // lua_settop(L, 0);

    in << content;
    return in;
}


int luaCell::kill(lua_State *luaL)
{
    int id = luaL_checknumber(luaL, 1);

    bool result = CellSubjectInterf::kill(id);
    lua_pushboolean(luaL, result);
    return 1;
}




/// Gets the luaCell position of the luaCell in the Lua stack
/// \param L is a pointer to the Lua stack
/// \param cell is a pointer to the cell within the Lua stack
void getReference( lua_State *L, luaCell *cell )
{
    cell->getReference(L);
}

