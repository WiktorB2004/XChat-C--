#include "../include/server_connection.h"
#include "../include/client.h"
#include <QObject>
#include <atomic>
#include <cstdlib>

// FIXME: DELETE/MOVE THIS VARIABLES INTO STRUCTS
// Define the message to send periodically
const char *curr_message;
// Global variable to store the connection state
enum ConnectionState connection_state = CLOSED;

// WebSocket callback function
static int callback_http(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    return 0;
}

static int callback_ws(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    // Get client pointer
    // std::atomic<Client *> client_ptr = reinterpret_cast<Client *>(lws_context_user(lws_get_context(wsi)));
    Client *client_ptr = (Client *)lws_context_user(lws_get_context(wsi));
    switch (reason)
    {
    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        std::cerr << "Connection error" << std::endl;
        break;

    case LWS_CALLBACK_CLIENT_ESTABLISHED:
        std::cout << "Connected to server: " + client_ptr->username << std::endl;
        connection_state = OPEN;
        emit client_ptr->connectionSuccess();
        break;

    case LWS_CALLBACK_CLIENT_RECEIVE:
        std::cout << "Received: " << (char *)in << std::endl;
        break;

    case LWS_CALLBACK_CLIENT_WRITEABLE:
        // Check if connection is open before sending message
        if (connection_state == OPEN)
        {
            std::cout << "TEST" << std::endl;
            // char *new_message = strdup(client_ptr->message.c_str());
            // if (!new_message)
            // {
            // // Handle memory allocation failure
            //     return -1;
            // }
            // curr_message = new_message;
            curr_message = "\"HELLO\"";
            // Send the selected message
            int len = strlen(curr_message);
            unsigned char buf[LWS_PRE + len];
            memcpy(buf + LWS_PRE, curr_message, len);
            lws_write(wsi, &buf[LWS_PRE], len, LWS_WRITE_TEXT);
        }
        break;

    case LWS_CALLBACK_CLOSED:
    case LWS_CALLBACK_CLIENT_CLOSED:
        std::cout << "Connection closed" << curr_message << std::endl;
        connection_state = CLOSED; // Update connection state
        break;

    default:
        break;
    }
    return 0;
}

int ServerConnection::run_server()
{
    // Server configuration
    struct lws_context_creation_info info;
    struct lws_client_connect_info ccinfo;
    struct lws_context *context;
    const char *address = server_address.c_str();
    int port = connection_port;
    int use_ssl = 0;
    int ietf_version = -1;

    // Define the protocols
    static const struct lws_protocols protocols[] = {
        {"http-only", callback_http, 0, 0},
        {"ws-only", callback_ws, 0, 0},
        {NULL, NULL, 0, 0} /* End of list */
    };

    memset(&info, 0, sizeof(info));
    memset(&ccinfo, 0, sizeof(ccinfo));

    lws_set_log_level(LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_INFO | LLL_DEBUG | LLL_CLIENT, NULL);

    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;
    info.gid = -1;
    info.uid = -1;
    info.user = client;

    context = lws_create_context(&info);
    if (!context)
    {
        std::cerr << "Failed to create LWS context" << std::endl;
        return -1;
    }

    ccinfo.context = context;
    ccinfo.address = address;
    ccinfo.port = port;
    ccinfo.path = "/ws";
    ccinfo.host = lws_canonical_hostname(context);
    ccinfo.origin = ccinfo.host;
    ccinfo.protocol = "ws-only";
    ccinfo.pwsi = NULL;
    ccinfo.ssl_connection = use_ssl;
    ccinfo.ietf_version_or_minus_one = ietf_version;

    struct lws *wsi = lws_client_connect_via_info(&ccinfo);
    Client *tmp_client = client.load();
    tmp_client->wsi = wsi;
    client.store(tmp_client);
    if (!wsi)
    {
        std::cerr << "Failed to connect to server" << std::endl;
        return -1;
    }

    // Main loop to run the service
    bool quit = false;
    while (!quit)
    {
        lws_service(context, 0);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    lws_context_destroy(context);

    return 0;
}
