// ConsoleApplication1.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

// Azure Storage Client Library for C++
// https://azure.github.io/azure-storage-cpp/

#include <iostream>
#include <was/storage_account.h>
#include <was/blob.h>
#include <cpprest/filestream.h>  
#include <cpprest/containerstream.h>

int main()
{
    // Define the connection-string with Azurite.
    const utility::string_t storage_connection_string(U("UseDevelopmentStorage=true;"));
    
    try
    {
        // Retrieve storage account from connection string.
        azure::storage::cloud_storage_account storage_account = azure::storage::cloud_storage_account::parse(storage_connection_string);

        // Create the blob client.
        azure::storage::cloud_blob_client blob_client = storage_account.create_cloud_blob_client();

        // Retrieve a reference to a container.
        azure::storage::cloud_blob_container container = blob_client.get_container_reference(U("my-sample-container"));

        // Create the container if it doesn't already exist.
        container.create_if_not_exists();

        // Retrieve reference to a blob named "my-blob-1".
        azure::storage::cloud_block_blob blockBlob = container.get_block_blob_reference(U("my-blob-1"));

        // Create or overwrite the "my-blob-1" blob with contents from a local file.
        concurrency::streams::istream input_stream = concurrency::streams::file_stream<uint8_t>::open_istream(U("DataFile.txt")).get();
        blockBlob.upload_from_stream(input_stream);
        input_stream.close().wait();

        // Create or overwrite the "my-blob-2" and "my-blob-3" blobs with contents from text.
        // Retrieve a reference to a blob named "my-blob-2".
        azure::storage::cloud_block_blob blob2 = container.get_block_blob_reference(U("my-blob-2"));
        blob2.upload_text(U("more text"));

        // Retrieve a reference to a blob named "my-blob-3".
        azure::storage::cloud_block_blob blob3 = container.get_block_blob_reference(U("my-directory/my-sub-directory/my-blob-3"));
        blob3.upload_text(U("other text"));

        // Output URI of each item.
        azure::storage::list_blob_item_iterator end_of_results;
        for (auto it = container.list_blobs(); it != end_of_results; ++it)
        {
            if (it->is_blob())
            {
                std::wcout << U("Blob: ") << it->as_blob().uri().primary_uri().to_string() << std::endl;
            }
            else
            {
                std::wcout << U("Directory: ") << it->as_directory().uri().primary_uri().to_string() << std::endl;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::wcout << U("Error: ") << e.what() << std::endl;
    }
}

