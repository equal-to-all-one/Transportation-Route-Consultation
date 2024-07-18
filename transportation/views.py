from .serializers import *
from rest_framework import viewsets, permissions, filters
from django_filters.rest_framework import DjangoFilterBackend
from .filters import DistanceFilter,FlightFilter
from rest_framework_bulk import BulkModelViewSet

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
    filter_backends = [DjangoFilterBackend,filters.OrderingFilter]
    filterset_class=FlightFilter
    permission_classes = (permissions.IsAuthenticatedOrReadOnly,)
    ordering_fields = ['time_cost','price']

    def perform_create(self, serializer):
        timecost = serializer.validated_data.get('arrive_time') - serializer.validated_data.get('depart_time')
        serializer.save(time_cost=timecost)


def get_fitCost(departure, destination, departime):
    pass


def inquire(request):
    if request.method == 'POST':
        departureCity = request.POST.get('departure_city')
        destinationCity = request.POST.get('destination_city')
        departTime = request.POST.get('depart_time')
        pass
