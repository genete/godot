#ifndef NETWORKED_MULTIPLAYER_ENET_H
#define NETWORKED_MULTIPLAYER_ENET_H

#include "io/networked_multiplayer_peer.h"
#include "enet/enet.h"
#include "io/compression.h"

class NetworkedMultiplayerENet : public NetworkedMultiplayerPeer {

	OBJ_TYPE(NetworkedMultiplayerENet,NetworkedMultiplayerPeer)
public:
	enum CompressionMode {
		COMPRESS_NONE,
		COMPRESS_RANGE_CODER,
		COMPRESS_FASTLZ,
		COMPRESS_ZLIB
	};
private:


	enum {
		SYSMSG_ADD_PEER,
		SYSMSG_REMOVE_PEER
	};

	bool active;
	bool server;

	uint32_t unique_id;

	int target_peer;
	TransferMode transfer_mode;

	ENetEvent event;
	ENetPeer *peer;
	ENetHost *host;

	bool refuse_connections;

	ConnectionStatus connection_status;

	Map<int,ENetPeer*> peer_map;

	struct Packet {

		ENetPacket *packet;
		int from;
	};

	CompressionMode compression_mode;

	mutable List<Packet> incoming_packets;

	mutable Packet current_packet;

	uint32_t _gen_unique_id() const;
	void _pop_current_packet() const;

	Vector<uint8_t> src_compressor_mem;
	Vector<uint8_t> dst_compressor_mem;

	ENetCompressor enet_compressor;
	static size_t enet_compress(void * context, const ENetBuffer * inBuffers, size_t inBufferCount, size_t inLimit, enet_uint8 * outData, size_t outLimit);
	static size_t  enet_decompress (void * context, const enet_uint8 * inData, size_t inLimit, enet_uint8 * outData, size_t outLimit);
	static void enet_compressor_destroy(void * context);
	void _setup_compressor();

protected:
	static void _bind_methods();
public:

	virtual void set_transfer_mode(TransferMode p_mode);
	virtual void set_target_peer(int p_peer);


	virtual int get_packet_peer() const;


	Error create_server(int p_port, int p_max_peers=32, int p_in_bandwidth=0, int p_out_bandwidth=0);
	Error create_client(const IP_Address& p_ip, int p_port, int p_in_bandwidth=0, int p_out_bandwidth=0);

	void close_connection();

	virtual void poll();

	virtual bool is_server() const;

	virtual int get_available_packet_count() const;
	virtual Error get_packet(const uint8_t **r_buffer,int &r_buffer_size) const; ///< buffer is GONE after next get_packet
	virtual Error put_packet(const uint8_t *p_buffer,int p_buffer_size);

	virtual int get_max_packet_size() const;

	virtual ConnectionStatus get_connection_status() const;

	virtual void set_refuse_new_connections(bool p_enable);
	virtual bool is_refusing_new_connections() const;

	virtual int get_unique_id() const;

	void set_compression_mode(CompressionMode p_mode);
	CompressionMode get_compression_mode() const;

	NetworkedMultiplayerENet();
	~NetworkedMultiplayerENet();
};

VARIANT_ENUM_CAST(NetworkedMultiplayerENet::CompressionMode);


#endif // NETWORKED_MULTIPLAYER_ENET_H
