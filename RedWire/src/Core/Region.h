#pragma once

#include <stdint.h>
#include <functional>
#include <memory>
#include "../Type2.h"
#include "IdField.h"
#include "Grid.h"

namespace RedWire
{
	struct Region : IdField
	{
		Region();
		Region(const Int2& size);

		uint32_t getColor(const Int2& position);
		inline const Int2& getSize() const { return size; }

		void setSize(const Int2& size);

		void copyFrom(const Int2& position, const IdField& field);
		void pasteTo(const Int2& position, IdField& field);

		void readFrom(std::istream& stream);
		void writeTo(std::ostream& stream) const;

		void rotatePositive();
		void rotateNegative();
		void rotate180();

	protected:
		uint8_t getId(const Int2& position) const override;
		void setId(const Int2& position, const uint8_t& id) override;

	private:
		size_t getIndex(const Int2& position) const;

		Int2 size;
		Grid colorsGrid;

		std::unique_ptr<uint8_t[]> ids;
	};
}