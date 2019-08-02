
#include "PDPCFarichBinaryDataReader.h"
#include "TreeFormats/PDPCFarichHitData.h"
#include "PDPCFarichFrameData.h"
#include "PDPCFarichGeometry.h"
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <exception>

using namespace std;

static const unsigned nChunkHits = 10000;

PDPCFarichBinaryDataReader::PDPCFarichBinaryDataReader(const char* fn) : PDPCFarichVDataReader(fn)
{
    fGeometry = NULL;
    file = NULL;

    buffer_size = 23*nChunkHits;
    buffer1 = new unsigned char[buffer_size];
}

PDPCFarichBinaryDataReader::~PDPCFarichBinaryDataReader()
{
    if( buffer1 ) delete [] buffer1;
    if( file ) Close();
}

void PDPCFarichBinaryDataReader::Open()
{
    flag_eof = kFALSE;
    flag_error = kFALSE;
    if( !filename || !*filename ) {
	cerr << "PDPCFarichBinaryDataReader::Open(): Filename is not specified" << endl;
	flag_error = kTRUE;
	return;
    }
    cout << "\nOpening binary data file " << filename << "..." << endl;
    file = fopen(filename, "rbS");
    if( !file ) {
	cerr << "PDPCFarichBinaryDataReader::Open(): Can not open file " << filename << endl;
	flag_error = kTRUE;
	return;
    }

    if( fread(buffer1,22,1,file)==0 ) {
	cerr << "PDPCFarichBinaryDataReader::Open(): Failed to read first bytes from " << filename << "\n  Reason: "
	<< (feof(file)?"EOF":(ferror(file)?"ERROR":"Unknown")) << endl;
	flag_error = kTRUE;
	return;
    }
    if( buffer1[0] == FirstCoincidenceHeader ) {
	SetCoincidenceMode(kTRUE);
	cout << "File contains data in coincidence mode" << endl;
    } else {
	SetCoincidenceMode(kFALSE);
	cout << "File contains data NOT in coincidence mode" << endl;
    }

    last_frame_seq_read =                     
        (uint32_t) buffer1[0] + ((uint32_t) buffer1[1] << 8) + ((uint32_t) buffer1[2] << 16) + ((uint32_t) buffer1[3] << 24);
    last_frame_read = (uint8_t) buffer1[4];
    last_sync_read = sync0 = (uint16_t) buffer1[5] + ((uint16_t) buffer1[6] << 8);
    
    cout << "First sync number is " << sync0 << endl;

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    cout << "File size is " << file_size << " bytes / " << file_size/hit_size << " hits." << endl;

    nsync_rollovers = 0;
    nhits_chunk = 0;
    nhits_total_read = 0;
    position = 0;
    file_position = 0;
}

void PDPCFarichBinaryDataReader::Close()
{
    if( !file ) return;
    cout << file_position << " bytes read from binary data file. Closing it... " << endl;
    fclose(file);
    file = NULL;
}

ULong_t PDPCFarichBinaryDataReader::ReadChunk()
{
    if( !file ) {
	cerr << "PDPCFarichBinaryDataReader::ReadChunk(): No file to read" << endl;
	return 0;
    }
    clearerr(file);
    // read next data from the file
    nhits_chunk = fread(buffer1, hit_size, buffer_size/hit_size, file);
    if( verbose )
        cout << "Chunk of " << nhits_chunk << " hits read from file position " << ftello(file) << endl;
    if( nhits_chunk==0 ) {
	if( ferror(file) ) {
	    flag_error = kTRUE;
	    cerr << "PDPCFarichBinaryDataReader::ReadChunk(): Error while reading data file" << endl;
	} else {
	    flag_eof = kTRUE;
	}
	return 0;
    }
    return nhits_chunk;
}

ULong_t PDPCFarichBinaryDataReader::SkipFrames(ULong_t nframes)
{
    if( verbose ) cout << "Skipping " << nframes << " requested" << endl;
  
    ULong_t iskipped = 0;  
    for( ; iskipped < nframes; iskipped++) {
	while( 1 ) {
	    if( position>=nhits_chunk*hit_size ) {
                position = 0;
                if( ReadChunk()==0 ) break; //EOF
	    }
	    if( binary_file_mode==CoincidenceMode ) {
		Int_t header = buffer1[position];
		if( file_position>0 && header==FirstCoincidenceHeader ) {
		    break;
		}
		if( header!=FirstCoincidenceHeader && header!=NextCoincidenceHeader ) { //check format of the header
		    cerr << "PDPCFarichBinaryDataReader::SkipFrames(): Wrong header word "
			<< hex << header << dec << " for hit " << file_position/hit_size << " in file" << endl;
		    flag_error = kTRUE;
		    return iskipped;
                }
	    } else {
	        UInt_t frame_sequence =
                    (uint32_t) buffer1[position+0] +
                    ((uint32_t) buffer1[position+1] << 8) +
                    ((uint32_t) buffer1[position+2] << 16) +
                    ((uint32_t) buffer1[position+3] << 24);
	        UInt_t frame = (uint8_t) buffer1[position+4];
                Int_t sync =
                    (uint32_t) buffer1[position+5] +
                    ((uint32_t) buffer1[position+6] << 8) +
                    ((uint32_t) nsync_rollovers<<16);

                if( sync<last_sync_read ) { //treat sync number roll-overs
                    nsync_rollovers++;
                    sync+=(1<<16);
                }
            
	        if( frame_sequence!=last_frame_seq_read ) { 
                    last_frame_seq_read = frame_sequence;
                    last_frame_read = frame;
                    last_sync_read = sync;
                    break;       
	        }
                last_frame_seq_read = frame_sequence;
                last_frame_read = frame;
                last_sync_read = sync;
	    }
            position += hit_size;
            file_position += hit_size;
	}
        if( nhits_chunk==0 ) break; //EOF
    }

    if( iskipped<nframes ) 
        cout << "File ended before requested number of frames is skipped. ";
    cout << "Skipped " << iskipped << (binary_file_mode==CoincidenceMode?" coincidences":" frames") << endl;

    return iskipped;
}

void PDPCFarichBinaryDataReader::ProcessFrame(PDPCFarichFrameData* frameData)
{
    if( !fGeometry ) {
	cerr << "PDPCFarichBinaryDataReader::ProcessFrame(): Geometry is not defined" << endl;
	return;
    }

    while( 1 ) {
        if( position>=nhits_chunk*hit_size ) {
            position = 0;
            if( ReadChunk()==0 ) return; //EOF
        }

        Bool_t first_hit = kFALSE;

        if( binary_file_mode==CoincidenceMode ) {
            Int_t header = (uint8_t) buffer1[position];
            if( header==FirstCoincidenceHeader ) {
                if( frameData->GetRawHitCollectionSize() ) //go to next coincidence
                    return;
                first_hit = kTRUE;
            }
            if( header!=FirstCoincidenceHeader && header!=NextCoincidenceHeader ) { //check format of the header
                cerr << "PDPCFarichBinaryDataReader::ProcessFrame(): Wrong header word "
                    << hex << header << dec << " for hit " << nhits_total_read << " in file" << endl;
                flag_error = kTRUE;
                return;
            }
            position++; //shift position after the header
        }

        //transfrom to little-endian format explicitely for portability
        UInt_t frame_sequence =
            (uint32_t) buffer1[position+0] +
            ((uint32_t) buffer1[position+1] << 8) +
            ((uint32_t) buffer1[position+2] << 16) +
            ((uint32_t) buffer1[position+3] << 24);
        UInt_t frame = (uint8_t) buffer1[position+4];

        if( frame_sequence==last_frame_seq_read && frame!=last_frame_read ) {
            cerr << "PDPCFarichBinaryDataReader::ProcessFrame(): frame number change for the same frame_sequence number:\n"
                 << " frame_sequence " << frame_sequence << ","
                 << " this frame " << frame << ", previous frame " << last_frame_read << endl;
            flag_error = kTRUE;
            return;
        }

        if( frame_sequence<last_frame_seq_read ) {
            cerr << "PDPCFarichBinaryDataReader::ProcessFrame(): Wrong sequence of frames:\n"
                << " this frame " << frame << ", previous frame " << last_frame_read << "\n"
                << " this frame_sequence " << frame_sequence << ", previous frame_sequence " << last_frame_seq_read << endl;
            flag_error = kTRUE;
            return;
        }

        Int_t sync =
            (uint32_t) buffer1[position+5] +
            ((uint32_t) buffer1[position+6] << 8) + 
            ((uint32_t) nsync_rollovers<<16);

        if( sync<last_sync_read ) { //treat sync number roll-overs
            nsync_rollovers++;
            sync+=(1<<16);
        }
        
        if( binary_file_mode==AllHitsMode && 
            frame_sequence!=last_frame_seq_read ) { //new frame
            if( frameData->GetRawHitCollectionSize() ) return; //next frame
            first_hit = kTRUE;
        } else if( nhits_total_read==0 ) {
            first_hit = kTRUE;
        }

        last_frame_seq_read = frame_sequence;
        last_frame_read = frame;
        last_sync_read = sync;

        UInt_t target =
            (uint16_t) buffer1[position+7] +
            ((uint16_t) buffer1[position+8] << 8);
        Int_t die = (uint8_t) buffer1[position+9];
        UInt_t timestamp =
            (uint32_t) buffer1[position+10]+
            ((uint32_t) buffer1[position+11] << 8) +
            ((uint32_t) buffer1[position+12] << 16) +
            ((uint32_t) buffer1[position+13] << 24);
        Int_t count1 =
            (uint16_t) buffer1[position+14] +
            ((uint16_t) buffer1[position+15] << 8);
        Int_t count2 =
            (uint16_t) buffer1[position+16] +
            ((uint16_t) buffer1[position+17] << 8);
        Int_t count3 =
            (uint16_t) buffer1[position+18] +
            ((uint16_t) buffer1[position+19] << 8);
        Int_t count4 =
            (uint16_t) buffer1[position+20] +
            ((uint16_t) buffer1[position+21] << 8);

        position += 22;
        file_position += hit_size;
        nhits_total_read++;

        Int_t bb = fGeometry->TranslateBusboardID(target);
        Int_t module = fGeometry->TranslateModuleID(target);
        Int_t tile = fGeometry->TranslateTileID(target);
        Int_t tileUID = fGeometry->GetTileUID(target);
        Int_t dieSID = fGeometry->GetDieSerialID(target,die);
        Int_t dieXindex = fGeometry->GetDieIndexX(dieSID);
        Int_t dieYindex = fGeometry->GetDieIndexY(dieSID);

        PDPCFarichHitData *hit = frameData->AddRawHit();
        hit->SetFirstHit(first_hit);
        hit->SetTarget(target);
        hit->SetTileUID(tileUID);
        hit->SetBusboard(bb);
        hit->SetModule(module);
        hit->SetTile(tile);
        hit->SetDie(die);
        hit->SetDieSerialID(dieSID);
        hit->SetFrameSequence(frame_sequence);
        hit->SetFrame(frame);
        hit->SetSync(sync-sync0+1);
        hit->SetTimestamp(timestamp);
        hit->SetCellCount(0,count1);
        hit->SetCellCount(1,count2);
        hit->SetCellCount(2,count3);
        hit->SetCellCount(3,count4);
        if( count1+count2+count3+count4==0 ) {
            cerr<<"PDPCFarichBinaryDataReader::ProcessFrame(): Illegal raw hit with zero amplitude"<<endl;
            if( !verbose || nhits_total_read>nHitsToPrint )
                hit->Print("H");
        }
        hit->SetXYindex(dieXindex,dieYindex);

        if( verbose && nhits_total_read<=nHitsToPrint ) {
            hit->Print("h");
        }
    }
}
