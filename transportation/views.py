from django.http import HttpResponse
import json,datetime
from .serializers import *
from rest_framework import viewsets, permissions, filters
from rest_framework.response import Response
from django_filters.rest_framework import DjangoFilterBackend
from .filters import DistanceFilter,FlightFilter
from rest_framework_bulk import BulkModelViewSet
from rest_framework.decorators import action
from rest_framework.decorators import api_view,renderer_classes
from rest_framework.renderers import JSONRenderer
import heapq
from datetime import datetime,timedelta
from django.middleware.csrf import get_token



def get_fitTime(departure, destination, departime):

    departure_city=City.objects.get(id=departure)
    destination_city=City.objects.get(id=destination)

    pq=[(timedelta(),departure_city,[])]

    shortest_times = {departime:timedelta()}

    while pq:
        current_time,current_city,path= heapq.heappop(pq)
        if current_city == destination_city:
            return path
        flights = FlightInfo.objects.filter(departure_city=current_city,depart_time__gte=departime+current_time)
        for flight in flights:
            next_city=flight.destination_city
            arrival_time=current_time+(flight.arrive_time-flight.depart_time)
            if next_city not in shortest_times or arrival_time<shortest_times[next_city]:
                shortest_times[next_city]=arrival_time
                heapq.heappush(pq,(arrival_time,next_city,path+[flight]))

    return []


class CityViewSet(viewsets.ModelViewSet):
    queryset = City.objects.all()
    serializer_class = CitySerializer
    permission_classes = (permissions.IsAuthenticatedOrReadOnly,)


class DistanceViewSet(BulkModelViewSet):
    queryset = Distance.objects.all()
    serializer_class = DistanceSerializer
    permission_classes = (permissions.IsAuthenticatedOrReadOnly,)
    filter_backends = [DjangoFilterBackend]
    filterset_class = DistanceFilter


class FlightViewSet(viewsets.ModelViewSet):
    queryset = FlightInfo.objects.all()
    serializer_class = FlightSerializer
    filter_backends = [DjangoFilterBackend, filters.OrderingFilter]
    filterset_class=FlightFilter
    permission_classes = (permissions.IsAuthenticatedOrReadOnly,)
    ordering_fields = ['time_cost', 'price']

    def perform_create(self, serializer):
        timecost = serializer.validated_data.get('arrive_time') - serializer.validated_data.get('depart_time')
        serializer.save(time_cost=timecost)

    @action(methods=['post'], detail=False, url_path='inquire')
    def inquire(self, request, *args, **kwargs):
        departureCity = request.data.get('departure_city')
        destinationCity = request.data.get('destination_city')
        departTimeStr = request.data.get('depart_time')
        departTime = datetime.strptime(departTimeStr,'%Y-%m-%d %H:%M')
        resList = get_fitTime(departureCity, destinationCity, departTime)
        serializer = self.get_serializer(resList, many=True)
        return Response(serializer.data)



"""
用不上了
def get_csrfToken(request):
    token=get_token(request)
    return HttpResponse(json.dumps({'token':token}), content_type='application/json, charset=utf-8')
"""

"""
@api_view(['POST'])
@renderer_classes([JSONRenderer])
def inquire(request):
    if request.method == 'POST':
        data=json.loads(request.body.decode('utf-8'))
        departureCity = data.get('departure_city')
        destinationCity = data.get('destination_city')
        departTimeStr = data.get('depart_time')
        departTime = datetime.strptime(departTimeStr,'%Y-%m-%d %H:%M')
        resList = get_fitTime(departureCity,destinationCity,departTime)
        resDict = {}
        for i in range(len(resList)):
            resDict[i]=resList[i]
        return Response(resDict)
    return Response({})
"""

