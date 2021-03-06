Neighborhood_ = {
    type_ = "Neighborhood",
        --Função que adiciona um vizinho à estrutura de vizinhança de uma célula recebendo como parâmetro uma referência para a célula.
	addNeighbor = function( self, cell, weight)
		if( not self:isNeighbor(cell) ) then
			return self.cObj_:addNeighbor( cell.x, cell.y, cell.cObj_, weight); 
		else
			self.cObj_:setNeighWeight( cell.x, cell.y, cell.cObj_, weight );
		end
  	end,
	addCell = function(self, index, cellularSpace, weight)
		if (weight == nil) then weight = {}; end
		-- problema sinistro com o segundo parametro, ele nao pode ser um iterador
		if self.cObj_ == nil then error("Error: self is nil!", 2); end
		if index.cObj_ == nil then error("Error: index is nil!", 2); end
		if cellularSpace.cObj_ == nil then error("Error: cellularSpace is nil!", 2); end
		return self.cObj_:addCell(index.cObj_, cellularSpace.cObj_, weight)
	end,
	--Funcao que adiciona um vizinho a estrutura de vizinhanca de uma celula recebendo como parametro uma referencia para a celula.
	clear = function(self)
		self.cObj_:clear()
	end,
	eraseCell = function(self, index)
		self.cObj_:eraseCell(index.cObj_)
	end,
	-- Funcao que retira uma celula da estrutura de vizinhanca de outra celula recebendo como parametro uma referencia para a celula.
	eraseNeighbor = function(self, cell)
		self.cObj_:eraseNeighbor(cell.x, cell.y, cell.cObj_)
	end,
	first = function(self)
		self.cObj_:first()
	end,
	getCellNeighbor = function(self, index)
		return self.cObj_:getCellNeighbor(index.cObj_)
	end,
	getCellWeight = function(self, index)
		return self.cObj_:getCellWeight  (index.cObj_)
	end,
	getCoord = function(self)
		return self.cObj_:getCoord()
	end,
	getID = function(self)
		return self.cObj_:getID()
	end,
	getNeighbor = function(self)
		return self.cObj_:getNeighbor()
	end,
	getNeighWeight = function(self, cell)
		return self.cObj_:getNeighWeight(cell.x, cell.y, cell.cObj_)
	end,
	getWeight = function(self)
		return self.cObj_:getWeight()
	end,
	isEmpty = function(self)
		return self.cObj_:isEmpty()
	end,
	isFirst = function(self)
		return self.cObj_:isFirst()
	end,
	isLast = function(self)
		return self.cObj_:isLast()
	end,
	isNeighbor = function(self, cell)
		return self.cObj_:isNeighbor(cell.x, cell.y, cell.cObj_)
	end,
	last = function(self)
		self.cObj_:last()
	end,
	next = function(self)
		self.cObj_:next()
	end,
	-- Funcao que reconfigura a estrutura de vizinhanca
	-- Parametros: 1 - cellularSpace: Espaco Celular
	--             2 - fCondition: Funcao que determina se a celula faz parte ou nao da vizinhanca
	--             3 - fWeight: Funcao que calcula o peso da relacao
	reconfigure = function(self, cellularSpace, fCondition, fWeight)
		self:first()
		while (not self:isLast()) do
			neighbor = self:getNeighbor()
			if (not fCondition(neighbor)) then
				self:eraseNeighbor(neighbor)
			end
			self:next()
		end
		for i, cell in ipairs(cellularSpace.cells) do
			if (fCondition(cell)) then
				self:addNeighbor(cell, fWeight(cell))
			end
		end
	end,
	sample = function(self, randomObj)
    local pos = nil
    if(randomObj and type(randomObj) == "Random") then
      pos = randomObj:integer(1, self:size())                          
    else
      pos = TME_GLOBAL_RANDOM:integer(1, self:size())            
    end

		local count = 1
		self:first()
		while (not self:isLast()) do
			neigh = self:getNeighbor()
			if count == pos then return neigh end
			self:next()
			count = count + 1
		end
	end,
	setCellWeight = function(self, index, weight)
		self.cObj_:setCellWeight(index.cObj_, weight)
	end,
	-- Metodo que altera o peso de uma relacao recebendo como parametro uma referencia para a celula vizinha.
	setNeighWeight = function(self, cell, weight)
		self.cObj_:setNeighWeight(cell.x, cell.y, cell.cObj_, weight)
	end,
	setWeight = function(self, weight)
		self.cObj_:setWeight(weight)
	end,
	size = function(self)
		return self.cObj_:size()
	end
}

local metaTableNeighborhood_ = {__index = Neighborhood_}

function Neighborhood(attrTab)
	if (attrTab == nil) then attrTab = {}; end
	local cObj = nil
	if (attrTab.cObj_ == nil) then
		cObj = TeNeighborhood()
		attrTab.cObj_ = cObj
	else
		cObj = attrTab.cObj_
	end
	setmetatable(attrTab, metaTableNeighborhood_)
	cObj:setReference(attrTab)
	return attrTab
end

