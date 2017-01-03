#pragma once


/**
 * MyFX struct ORDER
 *
 * Represents a single Metatrader order.
 */
struct ORDER {
   uint ticket;
};


typedef std::vector<ORDER> OrderVector;
typedef OrderVector        OrderHistory;
